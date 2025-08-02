#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[])
{

	if(argc != 3) errx(1, "Wrong number of arguments");

	int N = strtol(argv[1], NULL, 10);
	int D = strtol(argv[2], NULL, 10);

	if(N < 0 || D < 0) err(1, "Error");

	int toChild[2];
	int toParent[2];

	for(int i = 0; i < N; i++)
	{

		if(pipe(toChild) < 0) err(1, "Pipe error");
		if(pipe(toParent) < 0) err(1, "Pipe error");

		pid_t pid = fork();
		if(pid < 0) err(1, "Fork error");
		if(pid == 0)
		{

			close(toChild[1]);
			close(toParent[0]);

			uint8_t dummy = 0;
			if(read(toChild[0], &dummy, sizeof(uint8_t)) < 0) err(1, "Error");
			if(write(1, "DONG\n", strlen("DONG\n")) <0) err(1, "Error");
			if(write(toParent[1], &dummy, sizeof(uint8_t)) < 0) err(1, "Error");

			close(toChild[0]);
			close(toParent[1]);
			exit(0);
				
		}
		
		close(toChild[0]);
		close(toParent[1]);

		if(write(1, "DING\n", strlen("DING\n")) < 0) err(1, "Error");
		uint8_t dummy = 0;
		
		if(write(toChild[1], &dummy, sizeof(uint8_t)) < 0) err(1, "Error");
		if(read(toParent[0], &dummy, sizeof(uint8_t)) < 0) err(1, "Error");
		sleep(D);

		close(toParent[0]);
		close(toChild[1]);

	}	

	return 0;

}
