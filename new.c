#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, int *argv[]) {
	int i = 0;
	while(i < argc) {
	printf("arg[%d]: %s\n", i, argv[i]);
		i++;
	}

	return -1;
}
