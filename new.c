#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int main(int argc, int *argv[]) {
	struct my_msgbuf Buf;
    	int msqid;
    	key_t key;
    	
    	if ((key = ftok("fork_exec.c", 'B')) == -1) {
        	perror("ftok");
        	exit(1);
    	}

    	if ((msqid = msgget(key, 0644)) == -1) {
        	perror("msgget");
        	exit(1);
    	}

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

	printf("CLIENT: ready to receive messages, captain.\n");

    	//for(;;) {
        	if (msgrcv(msqid, &Buf, sizeof Buf.mtext, 0, 0) == -1) {
            		perror("msgrcv");
            		exit(1);
        	}
        	printf("CLIENT: \"%s\"\n", Buf.mtext);
    	//}
	
	int i = 0;
	while(i < argc) {
	printf("arg[%d]: %s\n", i, argv[i]);
		i++;
	}
	
	//read(server_to_client, buf, BUFSIZ);

	//printf("...received from the server: %s\n",str);

	close(client_to_server);
    	close(server_to_client);

	return 0;
}
