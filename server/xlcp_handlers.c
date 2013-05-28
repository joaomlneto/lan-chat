/* 
 * XL-COM Protocol Logic Layer
 * 
 * xlcp_handlers.c
 *
 * Implementation of the XL-COM server logic and service handlers. 
 * The handlers also have to format data from and to the standard
 * message formats associated with requests and responses.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xlcp_handlers.h"
#include "xlcom.h"

void xlcp_ping(client_request_t* request, xlcp_msg_res_t* res, int* ressz) {

	// no input arguments

	// format the response to the client
	*ressz = sizeof(*res) - sizeof(res->body);
	res->type = REQ_PING;
	res->status = RES_ERROR;

	// handle request
	if (xlcom_ping(request->client) == XLCOM_STAT_OK) {
		res->status = RES_OK;
	}

}

void xlcp_echo(client_request_t* request, xlcp_msg_res_t* res, int* ressz) {

	// get input arguments
	char* msg_in = request->msg.body.echo.msg;

	// format the response to the client
	*ressz = sizeof(*res) - sizeof(res->body);
	res->type = REQ_ECHO;
	res->status = RES_ERROR;

	// handle request
	if (xlcom_echo(request->client, msg_in, res->body.echo.msg) == XLCOM_STAT_OK) {
		*ressz = sizeof(*res) - sizeof(res->body) + sizeof(char) * (strlen(msg_in) + 1);
		res->status = RES_OK;
	}

}

void xlcp_auth(client_request_t* request, xlcp_msg_res_t* res, int* ressz) {

	// get input arguments
	char* username = request->msg.body.auth.user;
	char* password = request->msg.body.auth.pass;

	// format the response to the client
	*ressz = sizeof(*res) - sizeof(res->body);
	res->type = REQ_AUTH;
	res->status = RES_ERROR;

	// handle request
	if (xlcom_auth(request->client, username, password) == XLCOM_STAT_OK) {
		*ressz = sizeof(*res) - sizeof(res->body);
		res->status = RES_OK;
	}

}

void xlcp_send_message(client_request_t* request, xlcp_msg_res_t* res, int* ressz) {

	// get input arguments
	char* message = request->msg.body.sendmsg.message;
	int target_id = request->msg.body.sendmsg.target_id;
	int target_type = request->msg.body.sendmsg.target_type;

	// format the response to the client
	*ressz = sizeof(*res) - sizeof(res->body);
	res->type = REQ_SEND_MESSAGE;
	res->status = RES_ERROR;

	// handle request
	if (xlcom_send_message(request->client, target_id, target_type, message) == XLCOM_STAT_OK) {
		*ressz = sizeof(*res) - sizeof(res->body);
		res->status = RES_OK;
	}

}
