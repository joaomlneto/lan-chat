/* 
 * XL-COM Client-Side Communication Layer
 * 
 * xlcp_api.h
 *
 * Interface of the client-side communication layer.
 * 
 * Read the project specification for futher information about
 * the description of the services.
 */

#ifndef _XLCP_API_H_
#define _XLCP_API_H_

#include <xlcp_proto.h>


// status of service invocation
typedef enum {
	XLCP_STAT_OK = 0,
	XLCP_STAT_ERROR = -1
} xlcp_call_status_t;

/*
 * xlcp_connect: internal initialization of the API (e.g. socket).
 * - server_addr: server ip address
 * - server_port: server port
 * - sock_fd: socket file descriptor [out]
 *   returns: XLCP API request status
 */
xlcp_call_status_t xlcp_connect(char* server_addr, unsigned short server_port, int* sock_fd);

/*
 * xlcp_disconnect: disconnects from server
 * - sock_fd: socket file descriptor
 *   returns: XLCP API request status
 */
xlcp_call_status_t xlcp_disconnect(int sock_fd);

/*
 * xlcp_ping: pings the server.
 * - sock_fd: socket file descriptor
 * - time: elapsed time [out]
 *   returns: XLCP API request status
 */
xlcp_call_status_t xlcp_ping(int sock_fd);

/*
 * xlcp_echo: asks the server to echo a string
 * - sock_fd: socket file descriptor
 * - send_msg: the string to send
 * - receive_msg: the string received
 *   returns: XLCP API request status
 */
xlcp_call_status_t xlcp_echo(int sock_fd, char* send_msg, char** receive_msg);

/*
 * xlcp_auth: authenticates the client in the server
 * - sock_fd: socket file descriptor
 * - user: client's username
 * - pass: client's password, encoded in SHA1
 *   returns: XLCP API request status
 */
xlcp_call_status_t xlcp_auth(int sock_fd, char* user, char* pass);

/*
 * xlcp_sendmsg: sends a message to a connected user or group of users
 * - sock_fd: socket file descriptor
 * - target_id: target ID on the server
 * - target_type: the type of client (user, group)
 */
xlcp_call_status_t xlcp_send_message(int sock_fd, int target_id, int target_type, char* message);

#endif
