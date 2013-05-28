/* 
 * XL-COM Server
 * 
 * xlcom_server.c
 *
 * Server implementing XL-COM services.
 * Read the project specification for futher information.
 */

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <xlcp_proto.h>
#include "xlcom.h"
#include "xlcp_handlers.h"
#include "client_info.h"
#include "avltree.h"
#include "queue.h"
#include "db.h"

/*
 * XL-COM Services
 *
 * All services are registered in a global table which is
 * used during request resolution to find the handler that
 * serves the request
 *
 * service handlers are implemented in xlcom.c
 */

// The server port for clients to connect to
#define LISTEN_PORT 64787

// Maximum pending client connection requests there can exist at
// any given time
#define LISTEN_QUEUE 30

// Number of seconds to retry the bind request if port is in use
#define BIND_RETRY_DELAY 5

// Number of request handling threads
// Should not go over the number of cores we want the app to use
#define NUM_THR_HANDLERS 4

// Database information
#define DATABASE_HOSTNAME "localhost"
#define DATABASE_PORT 3306 // default MySQL port
#define DATABASE_USERNAME "xlcom"
#define DATABASE_PASSWORD "xlcom_secret"
#define DATABASE_NAME "xlcom"


/*
 * XL-COM Service Request Handlers
 */
typedef struct {
	xlcp_msg_type_t type;
	xlcp_handler_t handler;
} service_handler_t;


/*
 * Server Global variables
 */

// Table of request handling functions
#define NUM_SERVICE_HANDLERS 4
service_handler_t Service[] = {
	{REQ_PING, xlcp_ping},
	{REQ_ECHO, xlcp_echo},
	{REQ_AUTH, xlcp_auth},
	{REQ_SEND_MESSAGE, xlcp_send_message}
};

// Epoll service file descriptor - active TCP connections poller
static int Epoll_fd;

// Client requests thread-safe queue
static queue_t* Queue_client_requests;

// Client requests semaphore (for consumer threads - unlimited buffer)
static sem_t Sem_client_requests;

// Active client connections BST - Currently implemented as an AVL Tree
static avltree_t* Active_connections;
// Lock to prevent concurrent modifications of the Active_connections tree
static pthread_mutex_t Active_connections_mutex;

// Database connection
static db_t* Database;

/*
 * Auxiliary functions
 */

// Function to compare two integers
int int_comparator(const void* a, const void* b) {
	return *(int*)a - *(int*)b;
}

// Function to initialize database connection
void database_init() {

	// Initialize library
	Database = db_init();

	// Establish connection
	if (db_connect(Database, DATABASE_HOSTNAME, DATABASE_PORT, DATABASE_USERNAME, DATABASE_PASSWORD, DATABASE_NAME) == NULL) {
		printf("[xlcom_server] [M] Failed to connect to database.\n");
		exit(-1);
	}
	printf("[xlcom_server] [M] Database connection established: %s@%s:%d/%s\n", DATABASE_USERNAME, DATABASE_HOSTNAME, DATABASE_PORT, DATABASE_NAME);
}

/*
 * XL-COM Client Requests Handler
 */
void* thr_handler(void* arg) {

	client_request_t *request;
	xlcp_msg_res_t response;
	int response_size;
	size_t req_handler_id;
	int found_req_handler;
	int status;
	int i;

	// Misc initializations
	mysql_thread_init();

	while(1) {

		// Wait for a request
		sem_wait(&Sem_client_requests);
		request = queue_remove(Queue_client_requests);

		// Clean up response
		memset(&response, 0, sizeof(response));

		// Find request handler
		found_req_handler = 0;
		for (i=0; i<NUM_SERVICE_HANDLERS; i++) {
			if(request->msg.type == Service[i].type) {
				found_req_handler = 1;
				req_handler_id = i;
				break;
			}
		}

		// Serve the request
		if (!found_req_handler) {
			response.status = RES_UNKNOWN;
			response_size = sizeof(response) - sizeof(response.body);
			printf("[xlcom_server] [H] Unknown request.\n");
		}
		else {
			Service[req_handler_id].handler(request, &response, &response_size);
		}

		// Send response to client
		status = send(request->client->fd, &response, response_size, MSG_DONTWAIT);

		// Check send status
		if (status < 0) {
			printf("[xlcom_server] [H] Error sending response.\n");
		}
		if (status != response_size) {
			printf("[xlcom_server] [H] Message size mismatch.\n");
		}

		// Free request memory
		free(request);

	}

}


/*
 * XL-COM Connection Poller
 */
void* thr_poller(void* arg) {

	int status;
	int client_fd;
	struct epoll_event epoll_ev;
	client_request_t *request;

	while(1) {
		// wait forever (-1) for one (1) request
		if ((status = epoll_wait(Epoll_fd, &epoll_ev, 1, -1) != 1)) {
			printf("[xlcom_server] [P] epoll_wait returned %d, expecting 1\n", status);
		}

		// Initialize memory for a new request
		if ((request = malloc(sizeof(client_request_t))) == NULL) {
			printf("[xlcom_server] [P] malloc failed. No free memory?\n");
		}
		memset(request, 0, sizeof(client_request_t));

		// Get new request info
		client_fd = epoll_ev.data.fd;
		request->msg_size = recv(client_fd, (void*)&(request->msg), sizeof(request->msg), MSG_DONTWAIT);

		// Lookup user information
		if ((request->client = avl_lookup(Active_connections, &client_fd)) == NULL) {
			printf("[xlcom_server] [P] CRITICAL ERROR: Client is connected, but no record found. Rejecting request.\n");
			continue;
		}

		// Check if client disconnected
		if (request->msg_size == 0) {
			printf("[xlcom_server] [P] %s disconnected.\n", client_info_print_auth(request->client));
			avl_remove(Active_connections, &client_fd);
			if (epoll_ctl(Epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) != 0) {
				printf("[xlcom_server] [P] Error removing connection from epoll.\n");
			}
			close(client_fd);
			free(request);
			continue;
		}

		// Check if there was an error
		if (request->msg_size < 0) {
			printf("[xlcom_server] [P] Error receiving request from %s.\n", request->client->address);
			free(request);
			return NULL;
		}

		// Place request in the requests FIFO
		queue_add(Queue_client_requests, request);
		sem_post(&Sem_client_requests);
	}

}


/*
 * XL-COM Connection Listener
 */
void* thr_listener(void* arg) {
	int listen_fd, client_fd;
	struct sockaddr_in client_addr, server_addr;
	socklen_t client_sz = sizeof(client_addr);
	struct epoll_event epoll_ev;
	int result;
	int* key_tmp;
	client_info_t* info_tmp;

	// Open socket to listen for new clients
	if ((listen_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		printf("[xlcom_server] [L] cannot open socket.\n");
		return NULL;
	}

	// Initialize server address structure
	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(LISTEN_PORT);

	// Bind socket to listen port
	while(bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
		printf("[xlcom_server] [L] cannot bind port %d. trying again in %d seconds.\n", LISTEN_PORT, BIND_RETRY_DELAY);
		sleep(BIND_RETRY_DELAY);
	}

	// Listen on that port
	listen(listen_fd, LISTEN_QUEUE);
	printf("[xlcom_server] [L] Listening on port %d. Max pending connections %d.\n", LISTEN_PORT, LISTEN_QUEUE);

	while(1) {

		// Wait for new connection
		if ((client_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_sz)) < 0) {
			perror("[xlcom_server] [L] could not accept connection.\n");
			close(listen_fd);
			return NULL;
		}
		printf("[xlcom_server] [L] Received new connection from %s on fd %d.\n", inet_ntoa(client_addr.sin_addr), client_fd);

		// Add new connection to epoll
		epoll_ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
		epoll_ev.data.fd = client_fd;
		if ((result = epoll_ctl(Epoll_fd, EPOLL_CTL_ADD, client_fd, &epoll_ev)) != 0) {
			printf("[xlcom_server] [L] Error adding new connection to epoll queue.\n");
			close(listen_fd);
			return NULL;
		}

		// Insert new connection into Active_connections
		// Key = Client socket FD
		key_tmp = malloc(sizeof(int));
		*key_tmp = client_fd;
		// Info = client_info_t structure
		info_tmp = client_info_create();
		client_info_set_fd(info_tmp, client_fd);
		client_info_set_address(info_tmp, inet_ntoa(client_addr.sin_addr));
		client_info_set_username(info_tmp, "Anonymous");
		// Insert it on active connections database
		avl_insert(Active_connections, key_tmp, info_tmp);
	}

}


/*
 * XL-COM Server main
 */
int main(int argc, char **argv) {
	pthread_t pthr_listener;
	pthread_t pthr_poller;
	pthread_t pthr_handler[NUM_THR_HANDLERS];
	int i;

	// initialize global variables
	Epoll_fd = epoll_create1(0);
	Queue_client_requests = queue_create();
	sem_init(&Sem_client_requests, 0, 0);
	Active_connections = avl_create(&int_comparator);
	pthread_mutex_init(&Active_connections_mutex, NULL);

	// Initialize libraries
	database_init();
	xlcom_init(Database, Active_connections);

	// create handler threads
	for(i=0; i<NUM_THR_HANDLERS; i++) {
		if (pthread_create(&pthr_handler[i], NULL, thr_handler, (void*) NULL) != 0) {
			printf("[xlcom_server] [M] Error creating handler thread with id %d\n", i);
			return -1;
		}
	}

	// create poller thread
	if (pthread_create(&pthr_poller, NULL, thr_poller, (void*) NULL) != 0) {
		printf("[xlcom_server] [M] Error creating poller thread\n");
		return -1;
	}

	// create listener thread
	if (pthread_create(&pthr_listener, NULL, thr_listener, (void*) NULL) != 0) {
		printf("[xlcom_server] [M] Error creating listener thread\n");
		return -1;
	}

	printf("[xlcom_server] [M] Successful initialization.\n");
	printf("[xlcom_server] [M] Created threads: [%d|handler] [%d|poller] [%d|listener]\n", NUM_THR_HANDLERS, 1, 1);

	// wait for thread termination
	pthread_join(pthr_listener, (void**) NULL);
	printf("[xlcom_server] [M] Listener thread exited.\n");
	pthread_join(pthr_poller, (void**) NULL);
	printf("[xlcom_server] [M] Poller thread exited.\n");
	for(i=0; i<NUM_THR_HANDLERS; i++) {
		pthread_join(pthr_handler[i], (void**) NULL);
		printf("[xlcom_server] [M] Handler thread with id %d exited.\n", i);
	}

	printf("[xlcom_server] [M] Graceful termiation. Bye!\n");
	return 0;
}

