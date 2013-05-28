/* 
 * XL-COM Communication Protocol
 * 
 * xlcp_proto.h
 *
 * XL-COM protocol specification used both by the Client API and
 * Server. Namely it describes the format of the messages
 * exchanged between client-server: the request and response
 * messages.
 *
 * Read the project specification for futher information about
 * the description of the services.
 */

#ifndef _XLCP_PROTO_H_
#define _XLCP_PROTO_H_

/*
 * XL-COM Protocol Types and Macros
 */
#define MAX_ECHO_MSG_LENGTH 256
#define MAX_USERNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 40
#define MAX_NAME_LENGTH 64
#define MAX_NICK_LENGTH 32
#define MAX_CLAN_LENGTH 32
#define MAX_PHONE_LENGTH 16
#define MAX_EMAIL_LENGTH 128
#define MAX_MESSAGE_LENGTH 512


/*
 * XL-COM Message Codes
 *  - message identifier - xlcp_msg_req_type_t
 *  - status returned by server - xlcp_msg_res_status_t
 */

typedef enum {
	REQ_NULL = 0,
	REQ_PING = 1,
	REQ_ECHO = 2,
	REQ_AUTH = 3,
	REQ_SEND_MESSAGE = 4
} xlcp_msg_type_t;

typedef enum {
	RES_OK = 0,
	RES_BUSY = 1,
	RES_ERROR = 2,
	RES_UNKNOWN = 3
} xlcp_msg_res_status_t;


/*
 * XL-COM Echo
 *   - request message: xlcp_msg_req_echo_t
 *   - response message: xlcp_msg_res_echo_t
 */

typedef struct {
	char msg[MAX_ECHO_MSG_LENGTH];
} xlcp_msg_req_echo_t;

typedef struct {
	char msg[MAX_ECHO_MSG_LENGTH];
} xlcp_msg_res_echo_t;


/*
 * XL-COM Authenticate Client
 *   - request message: xlcp_msg_req_auth_t
 *   - response message: none
 */

typedef struct {
	char user[MAX_USERNAME_LENGTH];
	char pass[MAX_PASSWORD_LENGTH]; // encrypted with SHA1
} xlcp_msg_req_auth_t;


/*
 * XL-COM Send Message
 *   - request message: xlcp_msg_req_sendmsg_t
 *   - response message: none
 */

typedef struct {
	int target_id;
	int target_type;
	char message[MAX_MESSAGE_LENGTH];
} xlcp_msg_req_sendmsg_t;


/*
 * XL-COM Messages
 *
 * Messages have a common 'type' field indicating the type of
 * service that the request/response refers to; the union contains
 * all the possible message formats both referring to requests and
 * responses. Response messages have also the 'status' field
 * indicating if the service succeeded.
 *
 * - request message: xlcp_msg_req_t
 * - response message: xlcp_msg_res_t
 */

typedef struct {
	xlcp_msg_type_t type;
	union {
		xlcp_msg_req_echo_t echo;
		xlcp_msg_req_auth_t auth;
		xlcp_msg_req_sendmsg_t sendmsg;
	} body;
} xlcp_msg_req_t;

typedef struct {
	xlcp_msg_type_t type;
	xlcp_msg_res_status_t status;
	union {
		xlcp_msg_res_echo_t echo;
	} body;
} xlcp_msg_res_t;

#endif

