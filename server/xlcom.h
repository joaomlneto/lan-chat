/*
 * XL-COM Services Layer
 *
 * xlcom.h
 *
 * Interface to the XL-COM Services layer, implemented at the server side.
 * The following interface functions are provided
 *
 * Manages the internal organization of the whole XL-COM server system,
 * and provides the following interface functions to programmers.
 */

#ifndef _XLCOM_H_
#define _XLCOM_H_

#include "xlcp_handlers.h"
#include "avltree.h"
#include "client_info.h"
#include "db.h"

typedef enum {
	XLCOM_STAT_OK = 0,
	XLCOM_STAT_ERROR = -1
} xlcom_call_status_t;

/*
 * xlcom_init: initializes the XL-COM library
 * - db: database connection handler
 * - clients: active clients information structure
 *   returns: 0 if successful, -1 otherwise
 */
xlcom_call_status_t xlcom_init(db_t* db, avltree_t* clients);

/*
 * xlcom_client_connect: initializes client information on connection
 * - fd: client socket file descriptor
 *   returns: 0 if successful, -1 otherwise
 */
xlcom_call_status_t xlcom_client_connect(unsigned int fd);

/*
 * xlcom_client_disconnect: tear down the connection with the client
 * - fd: client socket file descriptor
 *   returns: 0 if successful, -1 otherwise
 */
xlcom_call_status_t xlcom_client_disconnect(unsigned int fd);

/*
 * xlcom_ping: responds to a ping request
 *   returns: 0 if successful, -1 otherwise
 */
xlcom_call_status_t xlcom_ping(client_info_t *client);

/*
 * xlcom_echo: echoes a string back to the client
 *   returns: 0 if successful, -1 otherwise
 */
xlcom_call_status_t xlcom_echo(client_info_t *client, char* in, char* out);

/*
 * xlcom_auth: authenticates a user
 *   returns: 0 if successful, -1 otherwise
 */
xlcom_call_status_t xlcom_auth(client_info_t *client, char* user, char* pass);

/*
 * xlcom_announce: broadcast a message to all users
 *   returns: 0 if successful, -1 otherwise
 */
xlcom_call_status_t xlcom_send_message(client_info_t *client, int target_id, int target_type, char* message);

#endif
