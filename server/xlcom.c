/*
 * XL-COM Services Layer
 *
 * xlcom.c
 *
 * Implementation of the XL-COM Services layer. Manages the internal organization
 * of the whole system. It implements the interface functions specified in xlcom.h
 */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "xlcom.h"
#include "client_info.h"
#include "db.h"
#include "avltree.h"

// Database to interact with
static db_t* Database;
static pthread_mutex_t Database_lock;

// Active connections (AVL Tree) - not used?
static avltree_t* Clients;

static int Lib_initialized = 0;

xlcom_call_status_t xlcom_init(db_t* db, avltree_t* clients) {
	// Check if library has already been initialized
	if (Lib_initialized) {
		printf("[xlcom_init] WARNING: Library has already been initialized.\n");
	}

	// Database structures
	Database = db;
	pthread_mutex_init(&Database_lock, NULL);

	// Active client connections, and client information
	Clients = clients;

	printf("[xlcom_init] library initialized.\n");
	Lib_initialized = 1;
	return XLCOM_STAT_OK;
}

xlcom_call_status_t xlcom_client_connect(unsigned int fd) {
	printf("[xlcom_client_connect] Boom.\n");
	return XLCOM_STAT_ERROR;
}

xlcom_call_status_t xlcom_client_disconnect(unsigned int fd) {
	printf("[xlcom_client_disconnect] Bang.\n");
	return XLCOM_STAT_ERROR;
}

xlcom_call_status_t xlcom_ping(client_info_t* client) {
	// Check if library is initialized
	if (!Lib_initialized) {
		printf("[xlcom_ping] library not initialized.\n");
		return XLCOM_STAT_ERROR;
	}
	printf("[xlcom_ping] request from %s.\n", client_info_print_auth(client));

	return XLCOM_STAT_OK;
}

xlcom_call_status_t xlcom_echo(client_info_t* client, char* in, char* out) {
	// Check if library is initialized
	if (!Lib_initialized) {
		printf("[xlcom_echo] library not initialized.\n");
		return XLCOM_STAT_ERROR;
	}
	printf("[xlcom_echo] request from %s for message '%s'.\n", client_info_print_auth(client), in);
	strncpy(out, in, MAX_ECHO_MSG_LENGTH);
	return XLCOM_STAT_OK;
}

xlcom_call_status_t xlcom_auth(client_info_t* client, char* user, char* pass) {

	db_result_t* result;
	db_row_t row;
	db_xlcom_user_t user_data;
	int found = 0;

	// Check if library is initialized
	if (!Lib_initialized) {
		printf("[xlcom_auth] library not initialized.\n");
		return XLCOM_STAT_ERROR;
	}

	// Query the database
	pthread_mutex_lock(&Database_lock);
	db_query_get_user_by_auth(Database, user, pass);
	result = db_get_result(Database);
	pthread_mutex_unlock(&Database_lock);

	// Check if there is a result
	while((row = db_get_row(result))) {
		user_data = db_row2user(row);
		printf("[xlcom_auth] Client at %s authenticated as %s.\n", client->address, user_data.username);
		client_info_set_id(client, user_data.id);
		client_info_set_username(client, user_data.username);
		client_info_set_is_authed(client, 1);
		found = 1;
	}

	return (found? XLCOM_STAT_OK : XLCOM_STAT_ERROR);
}

xlcom_call_status_t xlcom_send_message(client_info_t *client, int target_id, int target_type, char* message) {
	printf("[xlcom_send_message] Implementing, please wait. :)\n");
	return XLCOM_STAT_ERROR;
}

