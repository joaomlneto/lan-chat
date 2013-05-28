/* 
 * XL-COM Client API
 * 
 * xlcom_api.h
 *
 * XL-COM programming interface for the client. Provides functions to manipulate
 * the client, and sending requests to the server using the XL-COM protocol.
 */

#ifndef _XLCOM_API_H_
#define _XLCOM_API_H_

typedef unsigned int xlcom_fh_t;

typedef enum {
	XLCOM_STAT_OK = 0,
	XLCOM_STAT_ERROR = -1
} xlcom_call_status_t;

/*
 * xlcom_connect: connects to a XL-COM Server
 * - server_addr: ip address of the server
 * - server_port: server port to connect
 * - sock_fd: socket file descriptor [out]
 *   returns: XL-COM API request status
 */
xlcom_call_status_t xlcom_connect(char* server_addr, unsigned short server_port, int* sock_fd);

/*
 * xlcom_disconnect: disconnects from XL-COM Server
 * - sock_fd: socket file descriptor
 *   returns: XL-COM API request status
 */
xlcom_call_status_t xlcom_disconnect(int sock_fd);

/*
 * xlcom_ping: sends a ping request to the active server
 * - sock_fd: socket file descriptor
 * - time: elapsed time in seconds [out]
 *   returns: XL-COM API request status
 */
xlcom_call_status_t xlcom_ping(int sock_fd, double* time);

/*
 * xlcom_echo: sends an echo request to the active server
 * - sock_fd: socket file descriptor
 * - msg: the string to echo
 *   returns: XL-COM API request status
 */
xlcom_call_status_t xlcom_echo(int sock_fd, char* msg);

/*
 * xlcom_auth: authenticates the client in the server
 * - sock_fd: socket file descriptor
 * - user: client's username
 * - pass: client's password
 *   returns: XL-COM API request status
 */
xlcom_call_status_t xlcom_auth(int sock_fd, char* user, char* pass);

/*
 * xlcom_send_message: sends a message to a user or group of users
 * - sock_fd: socket file descriptor
 * - target_id: id of the receiver
 * - target_type: the type of the receiver
 * - message: the message to send to the receiver
 *   returns: XL-COM API request status
 */
xlcom_call_status_t xlcom_send_message(int sock_fd, int target_id, int target_type, char* message);

#endif 
