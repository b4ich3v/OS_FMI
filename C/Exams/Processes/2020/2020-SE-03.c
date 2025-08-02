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

typedef struct tuple 
{

	char name[8];
	uint32_t offset;
	uint32_t len;

} tuple;

int toParent[8][2];

int main(int argc, char* argv[])
{
	
	if(argc != 2) errx(1, "Wrong number of arguments");

	struct stat fileStat;
	if(stat(argv[1], &fileStat) < 0) err(1, "Stat error");
	if(fileStat.st_size / sizeof(tuple) > 8) err(1, "To much data");

	int readingFd = open(argv[1], O_RDONLY);
	if(readingFd < 0) err(1, "Read error");

	int countOfTuples = fileStat.st_size / sizeof(tuple);
	tuple current;

	for(int i = 0; i < countOfTuples; i++)
	{

		if(read(readingFd, &current, sizeof(tuple))<0) err(1, "Read error");
		
		int terminatingIndex = 0;
		for(int j = 0; j < 8; j++) if(current.name[j] == 0x00) { terminatingIndex = j; break; }
		current.name[terminatingIndex] = '\0';

		int fdFromTuple = open(current.name, O_RDONLY);
		if(fdFromTuple < 0) err(1, "Read error");
		if(pipe(toParent[i]) < 0) err(1, "Pipe error");

		pid_t pid = fork();
		if(pid < 0) err(1, "Fork error");
		if(pid == 0)
		{

			close(toParent[i][0]);

			if(lseek(fdFromTuple, sizeof(uint16_t) * current.offset, SEEK_SET) < 0) err(1, "Lseek error");
			uint16_t currentNumber = 0;
			uint16_t result = 0;
			int readBytes = 0;

			for(int j = 0; j < (int)current.len; j++)
			{
					
				if((readBytes = read(fdFromTuple, &currentNumber, sizeof(uint16_t))) < 0) err(1, "Read error");
				if(readBytes != sizeof(uint16_t)) err(1, "Wrong len");
				result ^= currentNumber;

			}

			if(write(toParent[i][1], &result, sizeof(uint16_t)) < 0) err(1, "Write error");
			close(toParent[i][1]);
			exit(0);

		}

		close(fdFromTuple);
		close(toParent[i][1]);

	}

	uint16_t finalResult = 0;

	for (int i = 0; i < countOfTuples; i++) 
	{

    	int status;
    	if (wait(&status) < 0) err(1, "Wait error");
    	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Status error");

	}

	for(int i = 0; i < countOfTuples; i++)
	{

		uint16_t currentResult = 0;
		if(read(toParent[i][0], &currentResult, sizeof(uint16_t))<0) err(1, "Read error");
		finalResult ^= currentResult;
		close(toParent[i][0]);

	}

	if(dprintf(1, "result: %04X\n", finalResult) < 0) err(1, "Dprintf error");
	close(readingFd);
	return 0;

}
