#include <stdlib.h>
#include <gtk/gtk.h>
#include <xlcom_api.h>
#include "client.h"

#include "ui_window_main.h"

static client_data_t* Client_data;

void client_init() {
	Client_data = malloc(sizeof(client_data_t));
	Client_data->connection = 0;
}

int client_connect(char* server_addr, unsigned short port) {
	return xlcom_connect(server_addr, port, &Client_data->connection);
}

int client_disconnect() {
	return xlcom_disconnect(Client_data->connection);
}

int main(int argc, char *argv[]) {

	// Initialize GTK library
	gtk_init(&argc, &argv);

	// Initialize client data
	client_init();

	// Initialize main window
	window_main_create();

	// Get everything working
	gtk_main();

	return 0;

}

