#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
	
	if(argc != 4) errx(1, "Wrong number of arguments");

	int N = strtol(argv[2], NULL, 10);
	if(N < 0 || N > (1 << 8)) err(1, "Invalid input");

	int writingFd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	int readingFd = open("/dev/random", O_RDONLY);
	if(readingFd < 0) err(1, "Open error");

	bool hasCrash = false;

	for(int i = 0; i < N; i++)
	{

		int sharedData[2];
		char result[100] = {0};

		uint16_t S = 0;
		if(read(readingFd, &S, sizeof(uint16_t)) < 0) err(1, "Read error");

		char* input = malloc(sizeof(char) * S + 1);
		if(input == NULL) err(1, "Null error");
		if(read(readingFd, input, S) < 0) err(1, "Read error");
		input[S] = '\0';
		
		if(pipe(sharedData) < 0) err(1, "Pipe error");
		if(write(sharedData[1], input, S) < 0) err(1, "Write error");

		pid_t currentPid = fork();
		if(currentPid < 0) err(1, "Fork error");
		if(currentPid == 0)
		{	

    		close(sharedData[1]);
    		if(dup2(sharedData[0], 0) < 0) err(1, "Dup2 error");
    		close(sharedData[0]);

    		int newExit = open("/dev/null", O_WRONLY);
    		if(newExit < 0) err(1, "Open error");

    		if(dup2(newExit, 1) < 0) err(1, "Dup2 error");
    		if(dup2(newExit, 2) < 0) err(1, "Dup2 error");
    		close(newExit);

    		execlp(argv[1], argv[1], (char*) NULL);
    		err(1, "Execlp error");

		}

		close(sharedData[0]);
		close(sharedData[1]);

		int status = 0;
		if(wait(&status) < 0) err(1, "Status error");
		if(WIFSIGNALED(status) != 0)
		{

			if(snprintf(result, sizeof(result), "%s %d\n", input, i) < 0) err(1, "Snprintf error");
			if(write(writingFd, result, strlen(result)) < 0) err(1, "Error");
			hasCrash = true;	

		}

		free(input);

	}
	
	close(writingFd);
	if(hasCrash) exit(42);	
	return 0;

}


