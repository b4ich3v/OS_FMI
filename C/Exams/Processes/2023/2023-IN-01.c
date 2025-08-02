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

int toParent[7][2];
int toChild[7][2];
const char pattern[3][4] = { "TIC\n", "TAC\n", "TOE\n" };

int main(int argc, char* argv[])
{

	if(argc != 3) errx(1, "Wrong number of arguments");

	int NC = strtol(argv[1], NULL, 10);
	int WC = strtol(argv[2], NULL, 10);

	if(NC < 1 || NC > 7 || WC < 1 || WC > 35) err(1, "NC or WC error");
	
	for(int i = 0; i < NC; i++) 
	{

		if(pipe(toParent[i]) < 0) err(1, "Pipe error");
		if(pipe(toChild[i]) < 0) err(1, "Child error");

		pid_t currentPid = fork();
		if(currentPid < 0) err(1, "Fork error"); 
		if(currentPid == 0)
		{

			close(toChild[i][1]);
			close(toParent[i][0]);

			while(true)
			{
			
				int count = 0;
				int readBytes = 0;

				if((readBytes = read(toChild[i][0], &count, sizeof(int))) < 0) err(1, "Read error");
				if(readBytes == 0) break;
				if(write(1, pattern[count % 3], 4) < 0) err(1, "Write error");	
				if(write(toParent[i][1], &count, sizeof(int)) < 0) err(1, "Write error");

			}

			close(toChild[i][0]);
			close(toParent[i][1]);
			exit(0);

		}

		close(toChild[i][0]);
		close(toParent[i][1]);

	}

	int countForWords = 0;
	int indexForProcess = 0;

	if(write(1, pattern[0], 4) < 0) err(1, "Write error");
	countForWords += 1;

	while(countForWords < WC)
	{

		int count = 0;
		if(write(toChild[indexForProcess][1], &countForWords, sizeof(int)) < 0) err(1, "Write error");
		if(read(toParent[indexForProcess][0], &count, sizeof(int)) < 0) err(1, "Read error");

		indexForProcess = (indexForProcess + 1) % NC;
		countForWords += 1;

	}

	for(int i = 0; i < NC; i++)
	{

		close(toChild[i][1]);
		close(toParent[i][0]);

	}	

	for (int i = 0; i < NC; i++)
	{
		
		int status = 0;
		if(wait(&status) < 0) err(1, "Wait error");
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Status error");

	}
	
	return 0;

}
