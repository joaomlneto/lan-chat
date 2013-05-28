/* 
 * XL-COM Protocol Logic Layer
 * 
 * xlcp_handlers.h
 *
 * Interface of the XL-COM Communication Protocol logic layer containing
 * the functions that manage the internal logic of XLCP and handles the
 * requests issued by clients.
 * 
 */

#ifndef _SNFS_HANDLERS_H_
#define _SNFS_HANDLERS_H_

#include <netinet/in.h>
#include <xlcp_proto.h>
#include "client_info.h"

/*
 * Client request type
 */
typedef struct {
	xlcp_msg_req_t msg;
	size_t msg_size;
	client_info_t* client;
} client_request_t;

/*
 * the xlcp request handler type
 * - request: incomming request
 * - res: the outgoing message (response) [out]
 * - ressz: the size of the outgoing message [out]
 */
typedef void (*xlcp_handler_t)(client_request_t *request, xlcp_msg_res_t *res, int* ressz);

/*
 * XL-COM Protocol Handlers
 *
 * All request handlers follow the above mentioned signature.
 * 
 */
void xlcp_ping(client_request_t *request, xlcp_msg_res_t *res, int* ressz);
void xlcp_echo(client_request_t *request, xlcp_msg_res_t *res, int* ressz);
void xlcp_auth(client_request_t *request, xlcp_msg_res_t *res, int* ressz);
void xlcp_send_message(client_request_t *request, xlcp_msg_res_t *res, int* ressz);

#endif
