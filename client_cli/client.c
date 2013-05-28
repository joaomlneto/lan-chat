#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <xlcom_api.h>
#include "bool.h"

#define BUFFER_SZ 1024

int Sock_fd;
bool Connected;

typedef enum {
	OPTION_EXIT,
	OPTION_CONNECT,
	OPTION_DISCONNECT,
	OPTION_PING,
	OPTION_ECHO,
	OPTION_AUTH,
	OPTION_SEND_MESSAGE,
} options;

void printmenu() {
	printf("##########################\n");
	printf("## XL-COM Client - Menu ##\n");
	printf("##########################\n");
	printf("(%d) - Connect to server\n", OPTION_CONNECT);
	printf("(%d) - Disconnect from server\n", OPTION_DISCONNECT);
	printf("(%d) - Send ping request\n", OPTION_PING);
	printf("(%d) - Send echo request\n", OPTION_ECHO);
	printf("(%d) - Authenticate\n", OPTION_AUTH);
	printf("(%d) - Send message\n", OPTION_SEND_MESSAGE);
	printf("(%d) - Exit\n", OPTION_EXIT);
	printf("\nOption = ");
}

void cmd_exit() {
	printf("Bye!\n");
	exit(0);
}

void cmd_connect() {
	char* ip = "127.0.0.1";
	unsigned short port = 64787;
	// Check if already connected
	if(Connected) {
		printf("Aborted. Already connected.\n");
		return;
	}
	// Try to connect
	if (xlcom_connect(ip, port, &Sock_fd) != XLCOM_STAT_OK) {
		printf("Error connecting to server.\n");
		return;
	}
	Connected = true;
	printf("Successfully connected.\n");
}

void cmd_disconnect() {
	// Check if connected
	if(!Connected) {
		printf("Aborted. Not connected.\n");
		return;
	}
	// Try to disconnect
	if(xlcom_disconnect(Sock_fd) != XLCOM_STAT_OK) {
		printf("Error disconnecting from server.\n");
		return;
	}
	Connected = false;
	printf("Successfully disconnected.\n");
}

void cmd_ping() {
	double time;
	// Check if connected
	if (!Connected) {
		printf("Aborted. Not connected.\n");
		return;
	}
	// Send request
	if (xlcom_ping(Sock_fd, &time) != XLCOM_STAT_OK) {
		printf("Error pinging the server.\n");
	}
	printf("Ping time = %f seconds\n", time);
}

void cmd_echo() {
	char sent_msg[100];
	printf("String to send: ");
	if(fgets(sent_msg, 100, stdin) == NULL) {
		printf("Error in fgets.\n");
		return;
	}
	// Check if connected
	if (!Connected) {
		printf("Aborted. Not connected.\n");
		return;
	}
	// Make string null-terminated
	sent_msg[strlen(sent_msg) - 1] = '\0';
	// Send request
	if (xlcom_echo(Sock_fd, sent_msg) != XLCOM_STAT_OK) {
		printf("Error echoing message.\n");
		return;
	}
	printf("Successfully echoed '%s'.\n", sent_msg);
}

void cmd_auth() {
	char user[100];
	char pass[100];
	// Read username
	printf("Username: ");
	if(fgets(user, 100, stdin) == NULL) {
		printf("Error in fgets.\n");
		return;
	}
	// Read password
	printf("Password: ");
	if(fgets(pass, 100, stdin) == NULL) {
		printf("Error in fgets.\n");
		return;
	}
	// Check if connected
	if (!Connected) {
		printf("Aborted. Not connected.\n");
		return;
	}
	// Make string null-terminated
	user[strlen(user)-1] = '\0';
	pass[strlen(pass)-1] = '\0';
	// Send request
	if (xlcom_auth(Sock_fd, user, pass) != XLCOM_STAT_OK) {
		printf("Error authenticating.\n");
		return;
	}
	printf("Successfully authenticated. Welcome, %s.\n", user);
}

void cmd_send_message() {
	int id;
	int type;
	char message[100];
	char buffer[10];

	// Read ID
	printf("Target ID: ");
	if (fgets(buffer, 9, stdin) == NULL) {
		printf("Error in fgets.\n");
		return;
	}
	id = atoi(buffer);

	// Read Type
	printf("Target type: ");
	if (fgets(buffer, 9, stdin) == NULL) {
		printf("Error in fgets.\n");
		return;
	}
	type = atoi(buffer);

	// Read message
	printf("Message: ");
	if (fgets(message, 100, stdin) == NULL) {
		printf("Error in fgets.\n");
		return ;
	}

	// Check if connected
	if (!Connected) {
		printf("Aborted. Not connected.\n");
		return;
	}

	// Make message string null-terminated
	message[strlen(message)-1] = '\0';

	// Send request
	if (xlcom_send_message(Sock_fd, id, type, message) != XLCOM_STAT_OK) {
		printf("Error sending message.\n");
		return ;
	}

	printf("Successfully sent message.\n");

}

int main() {
	char buffer[1024];
	int option = 0;

	Connected = false;
	Sock_fd = 0;

	do {

		printmenu();
		if(fgets(buffer, 1024, stdin) == NULL) {
			printf("ERROR IN FGETS\n");
			continue;
		}
		option = atoi(buffer);

		switch(option) {
			case OPTION_CONNECT: cmd_connect(); break;
			case OPTION_DISCONNECT: cmd_disconnect(); break;
			case OPTION_PING: cmd_ping(); break;
			case OPTION_ECHO: cmd_echo(); break;
			case OPTION_AUTH: cmd_auth(); break;
			case OPTION_SEND_MESSAGE: cmd_send_message(); break;
		}

	} while (option != OPTION_EXIT);

	printf("Bye!\n");
	return 0;
}
