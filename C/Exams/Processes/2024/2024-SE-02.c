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

int pids[11];
for (int i = 0; i < 11; i++) pids[i] = -1;

int main(int argc, char* argv[])
{
	
	if(argc < 2 || argc > 11) errx(1, "Wrong number of arguments");

	int N = argc;

	for(int i = 1; i < N; i++)
	{
		
		pid_t currentPid = fork();
		if(currentPid < 0) err(1, "Fork error");
		pids[i - 1] = currentPid;
		if(currentPid == 0) 
		{

			execlp(argv[i], argv[i], (char*) NULL);
			err(1, "Execlp error");

		}

	}

	int counter = N - 1;
	int i = 0;

	while(counter > 0)
	{

		if(pids[i] == -1) 
		{

    		i = (i + 1) % (N - 1);
    		continue;

		}

		int status = 0;
		if(waitpid(pids[i], &status, 0) < 0) err(1, "Wait error");

		if(WIFSIGNALED(status))
		{

			for(int j = 0; j < N - 1; j++)
			{
				
				if(pids[j] == -1) continue;
				if(kill(pids[j], SIGTERM) < 0) err(1, "Kill error");

			}

			exit(i + 1);

		}
		else if(!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		{

			if(kill(pids[i], SIGTERM) < 0) err(1, "Kill error");
			if(waitpid(pids[i], NULL, 0) < 0) err(1, "Wait error");

			pid_t newPid = fork();
			if(newPid < 0) err(1, "Fork error");
			pids[i] = newPid;
			if(newPid == 0)
			{

				execlp(argv[i + 1], argv[i + 1], (char*) NULL);
				err(1, "Execlp error");

			}

		}
		else 
		{
			
			pids[i] = -1;
			counter -= 1;

		}

		i = (i + 1) % (N - 1);

	}

	exit(0);

}

