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

int arrayWithPids[100] = {-1};

int main(int argc, char* argv[])
{

	int sharedData[100][2];	

	for(int i = 1; i < argc; i++)
	{

		if(pipe(sharedData[i - 1]) < 0) err(1, "Pipe error");

		pid_t currentPid = fork();
		if(currentPid < 0) err(1, "Fork error");
		arrayWithPids[i - 1] = currentPid;
	    if(currentPid == 0)
		{

			close(sharedData[i - 1][0]);
			if(dup2(sharedData[i - 1][1], 1) < 0) err(1, "Dup2 error");
			close(sharedData[i - 1][1]);

			execlp(argv[i], argv[i], (char*) NULL);
			err(1, "Execlp error");

		}

		close(sharedData[i - 1][1]);	

	}

	bool hasError = false;

	for(int i = 0; i < argc - 1; i++)
	{

		if(arrayWithPids[i] == -1) continue;

		int status = 0;
		char buffer[10] = {0};
		int readBytes = 0;

		if(waitpid(arrayWithPids[i], &status, 0) < 0) err(1, "Wait error");
		if((readBytes = read(sharedData[i][0], buffer, sizeof(buffer))) < 0) err(1, "Read error");
		buffer[readBytes] = '\0';

		arrayWithPids[i] = -1;
		close(sharedData[i][0]);

		if(!strcmp(buffer, "found it!"))
		{

			for(int j = 0; j < argc - 1; j++)
			{
				
				if(j == i || arrayWithPids[j] == -1) continue;
				kill(arrayWithPids[j], SIGTERM);

			}

			exit(0);

		}
		else if(!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		{

			hasError = true;

		}

	}
	
	if(hasError) exit(129);
	exit(1);

}
