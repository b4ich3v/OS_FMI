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

typedef struct pair
{
	uint32_t offset;
	uint32_t len;

} pair;


int main(int argc, char* argv[])
{
	
	if(argc != 4) errx(1, "Wrong number of arguments");

	int readingFd1 = open(argv[1], O_RDONLY);
	if(readingFd1 < 0 ) err(1, "Open error");

	int readingFd2 = open(argv[2], O_RDONLY);
	if(readingFd2 < 0) err(1, "Open error");

	int writingFd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error"); 

	struct stat fileStat;
	if(stat(argv[1], &fileStat) < 0) err(1, "Stat error");
	if(fileStat.st_size % sizeof(pair) != 0) err(1, "Invalid input"); 
	if(stat(argv[2], &fileStat) < 0) err(1, "Stat error");
	int sizeOfFile2 = fileStat.st_size;

	pair currentPair;

	while(read(readingFd1, &currentPair, sizeof(pair)) > 0)
	{
		
		if((int)(sizeof(uint32_t) * currentPair.offset + currentPair.len) > sizeOfFile2) err(1, "Wrong data");
		if(lseek(readingFd2, sizeof(uint32_t) * currentPair.offset, SEEK_SET) < 0) err(1, "Lseek error");
		
		for(int i = 0; i < (int)currentPair.len; i++)
		{

			uint32_t currentNumber = 0;
			if(read(readingFd2, &currentNumber, sizeof(uint32_t)) < 0) err(1, "Read errro");
			if(write(writingFd, &currentNumber, sizeof(uint32_t)) < 0) err(1, "Write error");

		}

	}	

	close(readingFd1);
	close(readingFd2);
	close(writingFd);
	return 0;

}

