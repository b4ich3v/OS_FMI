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

int main(void)
{

	char* commands[100][100] = 
	{

		{"cat", "/etc/passwd", NULL},
		{"cut", "-d", ":", "-f7", NULL},
		{"uniq", "-c", NULL},
		{"sort", "-t", " ", "-k1", "-n", NULL}

	};
	
	int countOfCommands = 4;
	int prevFd = -1;

	for(int i = 0; i < countOfCommands; i++)
	{

		int sharedData[2];

		if(i != countOfCommands - 1)
		{

			if(pipe(sharedData)<0) err(1, "Pipe error");

		}

		pid_t currentPid = fork();
		if(currentPid <0) err(1, "Fork error");
		if(currentPid ==0)
		{

			if(prevFd != -1)
			{

				if(dup2(prevFd, 0)<0) err(1, "Dup2 error");
				close(prevFd);

			}

			if(i != countOfCommands - 1)
			{

				close(sharedData[0]);
				if(dup2(sharedData[1], 1)<0) err(1, "Dup2 error");
				close(sharedData[1]);

			}

			execvp(commands[i][0], commands[i]);
			err(1, "Execvp error");

		}

		if(prevFd != -1) close(prevFd);

		if(i != countOfCommands - 1)
		{

			close(sharedData[1]);
			prevFd = sharedData[0];

		}

	}

	int status = 0;

	for (int i = 0; i < countOfCommands; i++)
	{
		
		if(wait(&status) < 0) err(1, "Wait error");
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Status error");

	}	

	return 0;

}

