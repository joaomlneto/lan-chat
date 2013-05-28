/*
 * XL-COM Client API
 *
 * xlcom_api.c
 *
 * Implementation of the XL-COM programming interface. This
 * interface uses the XL-COM Protocol to invoke services on
 * a remote server.
 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <xlcom_api.h>
#include <xlcp_api.h>
#include <xlcp_proto.h>

#include "bool.h"

xlcom_call_status_t xlcom_connect(char* server_addr, unsigned short port, int* sock_fd) {

	if(xlcp_connect(server_addr, port, sock_fd) == XLCP_STAT_ERROR) {
		printf("[xlcom_connect] Error on xlcp_init.\n");
		return XLCOM_STAT_ERROR;
	}

	printf("[xlcom_connect] Successfully connected.\n");
	return XLCOM_STAT_OK;

}

xlcom_call_status_t xlcom_disconnect(int sock_fd) {

	if(xlcp_disconnect(sock_fd) == XLCP_STAT_ERROR) {
		printf("[xlcom_disconnect] Error on xlcp_disconnect.\n");
		return XLCOM_STAT_ERROR;
	}

	printf("[xlcom_disconnect] Successfully disconnected.\n");
	return XLCOM_STAT_OK;

}

xlcom_call_status_t xlcom_ping(int sock_fd, double* time) {
	int status;
	struct timeval start_val, stop_val;
	double start, stop;

	gettimeofday(&start_val, NULL);
	status = xlcp_ping(sock_fd);
	gettimeofday(&stop_val, NULL);

	if(status != XLCP_STAT_OK) {
		printf("[xlcom_ping] Error in xlcp_ping.\n");
		return XLCOM_STAT_ERROR;
	}

	// Calculate elapsed time
	start = start_val.tv_sec + (start_val.tv_usec/1000000.0);
	stop = stop_val.tv_sec + (stop_val.tv_usec/1000000.0);
	*time = stop - start;

	return XLCOM_STAT_OK;

}

xlcom_call_status_t xlcom_echo(int sock_fd, char* send_msg) {
	int status;
	char* receive_msg;

	// Check message length
	if (strlen(send_msg) >= MAX_ECHO_MSG_LENGTH) {
		printf("[xlcom_echo] Message too big.\n");
		return XLCOM_STAT_ERROR;
	}

	receive_msg = malloc(MAX_ECHO_MSG_LENGTH * sizeof(char));

	status = xlcp_echo(sock_fd, send_msg, &receive_msg);

	if (status != XLCP_STAT_OK) {
		printf("[xlcom_echo] Error in xlcp_echo.\n");
		return XLCOM_STAT_ERROR;
	}

	// Check if both strings are equal
	if(strcmp(send_msg, receive_msg) != 0) {
		printf("[xlcom_echo] Received string is not the same as sent string.\n");
		return XLCOM_STAT_ERROR;
	}

	return XLCOM_STAT_OK;

}

xlcom_call_status_t xlcom_auth(int sock_fd, char* user, char* pass) {
	int status;

	// Check username length
	if (strlen(user) >= MAX_USERNAME_LENGTH) {
		printf("[xlcom_auth] Username length exceeded.\n");
		return XLCOM_STAT_ERROR;
	}

	// Check password length
	if (strlen(pass) >= MAX_PASSWORD_LENGTH) {
		printf("[xlcom_auth] Password length exceeded.\n");
		return XLCOM_STAT_ERROR;
	}

	// Send the request
	status = xlcp_auth(sock_fd, user, pass);

	// Check response status
	if (status != XLCP_STAT_OK) {
		printf("[xlcom_auth] Error in xlcp_auth.\n");
		return XLCOM_STAT_ERROR;
	}

	return XLCOM_STAT_OK;
}

xlcom_call_status_t xlcom_send_message(int sock_fd, int target_id, int target_type, char* message) {
	int status;

	// Check message length
	if (strlen(message) >= MAX_MESSAGE_LENGTH) {
		printf("[xlcom_send_message] Message length exceeded.\n");
		return XLCOM_STAT_ERROR;
	}

	// Send the request
	status = xlcp_send_message(sock_fd, target_id, target_type, message);

	// Check response status
	if (status != XLCP_STAT_OK) {
		printf("[xlcom_send_message] Error in xlcp_send_message.\n");
		return XLCOM_STAT_ERROR;
	}

	return XLCOM_STAT_OK;
}
