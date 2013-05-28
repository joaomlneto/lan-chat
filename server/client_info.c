#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client_info.h"

client_info_t* client_info_create() {
	client_info_t* info = malloc(sizeof(client_info_t));
	memset(info, 0, sizeof(client_info_t));
	pthread_mutex_init(&info->mutex, NULL);
	return info;
}

void client_info_destroy(client_info_t* info) {
	pthread_mutex_destroy(&info->mutex);
	free(info);
}

void client_info_set_fd(client_info_t* info, int fd) {
	pthread_mutex_lock(&info->mutex);
	info->fd = fd;
	pthread_mutex_unlock(&info->mutex);
}

int client_info_get_fd(client_info_t* info) {
	return info->fd;
}

void client_info_set_address(client_info_t* info, char* address) {
	pthread_mutex_lock(&info->mutex);
	strncpy(info->address, address, INET_ADDRSTRLEN); // INET6_ADDRSTRLEN for IPv6
	pthread_mutex_unlock(&info->mutex);
}

char* client_info_get_address(client_info_t* info) {
	return info->address;
}

void client_info_set_is_authed(client_info_t* info, int is_authed) {
	pthread_mutex_lock(&info->mutex);
	info->is_authed = is_authed;
	pthread_mutex_unlock(&info->mutex);
}

int client_info_get_is_authed(client_info_t* info) {
	return info->is_authed;
}

void client_info_set_id(client_info_t* info, int id) {
	pthread_mutex_lock(&info->mutex);
	info->id = id;
	pthread_mutex_unlock(&info->mutex);
}

int client_info_get_id(client_info_t* info) {
	return info->id;
}

void client_info_set_username(client_info_t* info, char* username) {
	pthread_mutex_lock(&info->mutex);
	strncpy(info->username, username, MAX_USERNAME_LEN);
	pthread_mutex_unlock(&info->mutex);
}

char* client_info_get_username(client_info_t* info) {
	return info->username;
}

char* client_info_print_auth(client_info_t* info) {
	static char message[100];
	pthread_mutex_lock(&info->mutex);
	if (client_info_get_is_authed(info)) {
		sprintf(message, "%s@%s", info->username, info->address);
	}
	else {
		sprintf(message, "%s", info->address);
	}
	pthread_mutex_unlock(&info->mutex);
	return message;
}
