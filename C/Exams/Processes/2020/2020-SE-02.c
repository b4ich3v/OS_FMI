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

	int writingFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Write error");

	int sharedData[2];
	if(pipe(sharedData) < 0) err(1, "Pipe error");

	pid_t pid = fork();
	if(pid < 0) err(1, "Fork error");
	if(pid == 0)
	{

		close(sharedData[0]);
		if(dup2(sharedData[1], 1) < 0) err(1, "Dup2 error");
		close(sharedData[1]);

		execlp("cat", "cat", argv[1], (char*) NULL);
		err(1, "Execlp error");
	
	}

	int status = 0;
	if(wait(&status) < 0) err(1, "Wait error");
	if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Status error");

	close(sharedData[1]);
	char currentByte = 'a';

	while(read(sharedData[0], &currentByte, sizeof(char)) > 0)
	{

		if(currentByte == 0x55) 
		{

			char dummy = 0x00;

			if(write(writingFd, &dummy, sizeof(char)) < 0) err(1, "Write error");
			continue;

		}
		else if(currentByte == 0x7D)
		{

			if(read(sharedData[0], &currentByte, sizeof(char)) < 0) err(1, "Read error");
			currentByte ^= 0x20;
			if(write(writingFd, &currentByte, sizeof(char)) < 0) err(1, "Write error");
			continue;

		}
		
		if(write(writingFd, &currentByte, sizeof(char)) < 0) err(1, "Write error");

	}

	close(sharedData[0]);
	close(writingFd);
	return 0;

}
