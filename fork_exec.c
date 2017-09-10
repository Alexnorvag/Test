#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

void ChildProcess();

int main(int argc, char* argv[]) {

	int exec_number = atoi(argv[1]);
	if (exec_number != 1 && exec_number != 2) {
		printf("Invalid input. Try this: ./name.o [number_executable_client]");
		return 0;
	}
	
	struct my_msgbuf Buf;
    	int msqid;
    	key_t key;
    	
    	if ((key = ftok("fork_exec.c", 'B')) == -1) {
        	perror("ftok");
        	exit(1);
    	}
    	
    	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        	perror("msgget");
        	exit(1);
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

	if (pid == 0 && exec_number == 1) {
		printf("CLIENT: 1\n");
		ChildProcess();
	}
	if (pid > 0) {
	
		printf("SERVER: STARTED\n");
	
		if (exec_number == 1) {
			
			client_to_server = open(myfifo1, O_RDONLY);
   			server_to_client = open(myfifo2, O_WRONLY);
      			//msgque
   			printf("Enter lines of text, ^D to quit:\n");
			Buf.mtype = 1;
	
			/*while(*/fgets(Buf.mtext, sizeof Buf.mtext, stdin);// != NULL) {
        		int len = strlen(Buf.mtext);

        		if (Buf.mtext[len-1] == '\n') Buf.mtext[len-1] = '\0';

        		if (msgsnd(msqid, &Buf, len+1, 0) == -1) /* +1 for '\0' */
            			perror("msgsnd");
    			//}

   			if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        			perror("msgctl");
        			exit(1);
    			}
   			//endmsgque
			waitpid(pid, 0, 0);
			printf("AFTERSERVER\n");
			
			read(client_to_server, buf, BUFSIZ);
			
			if (strcmp("", buf) != 0) {
         			printf("Received: %s\n", buf);
         			//write(server_to_client, strI, sizeof(strI));
      			}			 
			
			close(client_to_server);
   			close(server_to_client);
   	
		}
		
		pid = fork();
		if (pid == 0 && exec_number == 2) {
			printf("CLIENT: 2\n");
			ChildProcess();
		}
		if (pid > 0 && exec_number == 2){
		
			client_to_server = open(myfifo1, O_RDONLY);
   			server_to_client = open(myfifo2, O_WRONLY);
			
			//msgque
   			printf("Enter lines of text, ^D to quit:\n");
			Buf.mtype = 1;
	
			/*while(*/fgets(Buf.mtext, sizeof Buf.mtext, stdin); //!= NULL) {
        		int len = strlen(Buf.mtext);

        		if (Buf.mtext[len-1] == '\n') Buf.mtext[len-1] = '\0';

        		if (msgsnd(msqid, &Buf, len+1, 0) == -1) /* +1 for '\0' */
            			perror("msgsnd");
    			//}

   			if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        			perror("msgctl");
        			exit(1);
    			}
   			//endmsgque
			
			waitpid(pid, 0, 0);
			
			printf("AFTERSERVER\n");
			read(client_to_server, buf, BUFSIZ);
			
			if (strcmp("", buf) != 0)
      			{
         			printf("Received: %s\n", buf);
         			//write(server_to_client,buf,BUFSIZ);
      			}	
      			
      			//memset(buf, 0, sizeof(buf));
      			
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
