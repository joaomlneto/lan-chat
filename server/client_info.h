/*
 * Client Information
 *
 * client_info.h
 *
 * Interface to manipulate client_info structures, which contains
 * information about a connected client.
 *
 */

#ifndef _CLIENT_INFO_H_
#define _CLIENT_INFO_H_

#include <netinet/in.h>
#include <pthread.h>

#define MAX_USERNAME_LEN 32

/*
 * Client information type
 */
typedef struct {
	pthread_mutex_t mutex;
	int fd;
	char address[INET_ADDRSTRLEN];
	int is_authed;
	int id;
	char username[MAX_USERNAME_LEN];
} client_info_t;

client_info_t* client_info_create();
void client_info_destroy(client_info_t* info);

void client_info_set_fd(client_info_t* info, int fd);
int client_info_get_fd(client_info_t* info);

void client_info_set_address(client_info_t* info, char* address);
char* client_info_get_address(client_info_t* info);

void client_info_set_is_authed(client_info_t* info, int is_authed);
int client_info_get_is_authed(client_info_t* info);

void client_info_set_id(client_info_t* info, int id);
int client_info_get_id(client_info_t* info);

void client_info_set_username(client_info_t* info, char* username);
char* client_info_get_username(client_info_t* info);

char* client_info_print_auth(client_info_t* info);

#endif
