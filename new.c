#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, int *argv[]) {
	int client_to_server;
   	char *myfifo1 = "/tmp/client_to_server_fifo";

   	int server_to_client;
   	char *myfifo2 = "/tmp/server_to_client_fifo";

   	char str[BUFSIZ];
   	printf("Input message to server: ");
   	scanf("%s", str);

	client_to_server = open(myfifo1, O_WRONLY);
   	server_to_client = open(myfifo2, O_RDONLY);
   	write(client_to_server, str, sizeof(str));

	int i = 0;
	while(i < argc) {
	printf("arg[%d]: %s\n", i, argv[i]);
		i++;
	}

	close(client_to_server);
    	close(server_to_client);

	return 0;
}
