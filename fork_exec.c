#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

void ChildProcess();

int main(int argc, char* argv[]) {
	int exec_number = atoi(argv[1]);
	if (exec_number != 1 && exec_number != 2) {
		printf("Invalid input. Try this: ./name.o [number_executable_client]");
		return 0;
	}
	
	int client_to_server;
	char *myfifo1 = "/tmp/client_to_server_fifo";

	int server_to_client;
	char *myfifo2 = "/tmp/server_to_client_fifo";

	char buf[BUFSIZ];
        
        unlink(myfifo1);
   	unlink(myfifo2);
        //delayem fifo
        if (mkfifo(myfifo1, 0666) != 0) {
        	perror("mkfifo");
        }
   	if (mkfifo(myfifo2, 0666) != 0) {
   		perror("mkfifo");
   	}
   	
	int pid;
	pid = fork();	

	if (pid == 0) {
		if (exec_number == 1) {
			printf("CLIENT: 1\n");
			ChildProcess();
		} else exit(1);
	}
	if (pid > 0) {
		printf("SERVER: STARTED\n");
		
		client_to_server = open(myfifo1, O_RDONLY);
   		server_to_client = open(myfifo2, O_WRONLY);
   		
		waitpid(pid, 0, 0);
		
		printf("AFTERSERVER\n");
		read(client_to_server, buf, BUFSIZ);

		if (strcmp("",buf)!=0)
      		{
         		printf("Received: %s\n", buf);
         		//write(server_to_client,buf,BUFSIZ);
      		}

		close(client_to_server);
   		close(server_to_client);
		
		pid = fork();
		if (pid == 0) {
			if (exec_number == 2) {
				printf("CLIENT: 2\n");
				ChildProcess();
			} else exit(1);
		}
		if (pid > 0) {
			client_to_server = open(myfifo1, O_RDONLY);
   			server_to_client = open(myfifo2, O_WRONLY);

			waitpid(pid, 0, 0);
			read(client_to_server, buf, BUFSIZ);
			if (strcmp("",buf)!=0)
      			{
         			printf("Received: %s\n", buf);
         			//write(server_to_client,buf,BUFSIZ);
      			}			
			
			close(client_to_server);
   			close(server_to_client);
		}
	}

	return 0;
}

void  ChildProcess()
{	
	char *argv[] = {"echo","Foo is my name.", NULL};
	execv("new.o", argv);
}
