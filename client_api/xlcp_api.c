/* 
 * XL-COM Communication Protocol API Layer
 * 
 * xlcp_api.c
 *
 * Implementation of the client-side communication layer.
 * 
 * Read the XL-COM Protocol API interface (xlcp_api.h) and the project specification 
 * for futher information about the services.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>

#include <xlcp_api.h>
#include <xlcp_proto.h>

/*
 * Internal private auxiliary functions
 */

/*
 * Makes a remote call, sending 'req' to the server and waiting for the response.
*/

static int remote_call(int sock_fd, xlcp_msg_req_t *req, int reqsz, xlcp_msg_res_t *res, int ressz) {

	int status = send(sock_fd, (void*)req, reqsz, 0);
	if (status < 0) {
		printf("[xlcp_api] sendto error: %s.\n", strerror(errno));
		return -1;
	}

	// waits for an answer
	status = recvfrom(sock_fd, res, ressz, 0, NULL, NULL);
	if (status < 0) {
		printf("[xlcp_api] recvfrom error: %s.\n", strerror(errno));
		return -1;
	}
	if (status == 0) {
		printf("[xlcp_api] server is closed.\n");
		return -1;
	}
	return status;
}

/*
 * XL-COM Communication Protocol API implementation (see xlcp_api.h)
 */

xlcp_call_status_t xlcp_connect(char* server_host, unsigned short server_port, int* sock_fd) {

	struct hostent *server;
	struct sockaddr_in server_addr;

	// creates TCP socket domain Internet
	if ((*sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { // use getprotobyname instead of IPPROTO_TCP
		printf("[xlcp_connect] socket creation error: %s.\n", strerror(errno));
		return XLCP_STAT_ERROR;
	}

	// get the server IP address
	server = gethostbyname(server_host);
	if (server == NULL) {
		printf("[xlcp_connect] no such host as %s.\n", server_host);
		return XLCP_STAT_ERROR;
	}

	// server sockaddr cleaning and initialization
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // IPv4. Use AF_INET6 for IPv6
	memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	server_addr.sin_port = htons(server_port);

	// establish the connection to the server
	if (connect(*sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("[xlcp_connect] failed to connect.\n");
		return XLCP_STAT_ERROR;
	}

	return XLCP_STAT_OK;
}

xlcp_call_status_t xlcp_disconnect(int sock_fd) {

	if(close(sock_fd) != 0) {
		printf("[xlcp_disconnect] Error closing socket.\n");
		return XLCP_STAT_ERROR;
	}

	return XLCP_STAT_OK;
}

xlcp_call_status_t xlcp_ping(int sock_fd) {
	xlcp_msg_req_t req;
	xlcp_msg_res_t res;

	memset(&req,0,sizeof(req));
	memset(&res,0,sizeof(res));

	// format request
	req.type = REQ_PING;

	int status = remote_call(sock_fd, &req, sizeof(req.type), &res, sizeof(res));

	// format response
	if (status < 0 || res.status != RES_OK) {
		return XLCP_STAT_ERROR;
	}

	return XLCP_STAT_OK;
}

xlcp_call_status_t xlcp_echo(int sock_fd, char* send_msg, char** receive_msg) {
	xlcp_msg_req_t req;
	xlcp_msg_res_t res;

	memset(&req, 0, sizeof(req));
	memset(&res, 0, sizeof(res));

	// format request
	req.type = REQ_ECHO;
	strncpy(req.body.echo.msg, send_msg, MAX_ECHO_MSG_LENGTH - 1);

	int status = remote_call(sock_fd, &req, sizeof(req.type) + sizeof(req.body.echo), &res, sizeof(res));

	// format response
	if (status < 0 || res.status != RES_OK) {
		return XLCP_STAT_ERROR;
	}

	strncpy(*receive_msg, res.body.echo.msg, MAX_ECHO_MSG_LENGTH - 1);
	return XLCP_STAT_OK;
}

xlcp_call_status_t xlcp_auth(int sock_fd, char* user, char* pass) {
	xlcp_msg_req_t req;
	xlcp_msg_res_t res;

	memset(&req, 0, sizeof(req));
	memset(&res, 0, sizeof(res));

	// format request
	req.type = REQ_AUTH;
	strncpy(req.body.auth.user, user, MAX_USERNAME_LENGTH - 1);
	strncpy(req.body.auth.pass, pass, MAX_PASSWORD_LENGTH - 1);

	int status = remote_call(sock_fd, &req, sizeof(req.type) + sizeof(req.body.auth), &res, sizeof(res));

	// format response
	if (status < 0 || res.status != RES_OK) {
		return XLCP_STAT_ERROR;
	}

	return XLCP_STAT_OK;

}

xlcp_call_status_t xlcp_send_message(int sock_fd, int target_id, int target_type, char* message) {
	xlcp_msg_req_t req;
	xlcp_msg_res_t res;

	memset(&req, 0, sizeof(req));
	memset(&res, 0, sizeof(res));

	// format request
	req.type = REQ_SEND_MESSAGE;
	req.body.sendmsg.target_id = target_id;
	req.body.sendmsg.target_type = target_type;
	strncpy(req.body.sendmsg.message, message, MAX_MESSAGE_LENGTH - 1);

	int status = remote_call(sock_fd, &req, sizeof(req.type) + sizeof(req.body.sendmsg), &res, sizeof(res));

	// format response
	if (status < 0 || res.status != RES_OK) {
		return XLCP_STAT_ERROR;
	}

	return XLCP_STAT_OK;

}
