#ifndef _CLIENT_H_
#define _CLIENT_H_

typedef struct {
	int connection;
} client_data_t;

void client_init();
int client_connect(char* server_addr, unsigned short port);
int client_disconnect();
int client_get_connection();

#endif
