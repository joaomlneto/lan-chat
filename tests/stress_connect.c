#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xlcom_api.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 64787

int main(int argc, char **argv)
{
   int* fd;
	int num_connections;
	int result;

	if (argc != 2) {
		printf("Usage: %s <num_connections>\n", argv[0]);
		return 0;
	}

	num_connections = atoi(argv[1]);
	fd = malloc(num_connections*sizeof(int));

	for(int i=0; i < num_connections; i++) {
		printf("%d/%d connections established.\n", i+1, num_connections);
		result = xlcom_connect(SERVER_ADDR, SERVER_PORT, &fd[i]);
		if (result != XLCOM_STAT_OK) {
			printf("Failed to connect num %d.\n", i+1);
			sleep(1);
			return -1;
		}
	}

	printf("Success on establishing %d connections\n", num_connections);
	printf("Going to close all connections.\n");

	sleep(2);

	for(int i=0; i < num_connections; i++) {
		printf("%d/%d connections disconnected.\n", i+1, num_connections);
		result = xlcom_disconnect(fd[i]);
		/*if (result != XLCOM_STAT_OK) {
			printf("Failed to disconnect num %d.\n", i+1);
			sleep(1);
			return -1;
		}*/
		close(fd[i]);
	}

	return 0;

}
