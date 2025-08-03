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

typedef struct helper
{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
} helper;

int main(int argc, char* argv[])
{
	
	if(argc != 4) errx(1, "Wrong number of arguments");

	int readingFd1 = open(argv[1], O_RDONLY);
	if(readingFd1 < 0) err(1, "Open error");

	int readingFd2 = open(argv[2], O_RDONLY);
	if(readingFd2 < 0) err(1, "Open error");

	int writingFd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	struct stat fileStat;
	if(stat(argv[1], &fileStat) < 0) err(1, "Stat error");
	int size1 = fileStat.st_size;
	if(stat(argv[2], &fileStat) < 0) err(1, "Stat error");
	int size2 = fileStat.st_size;
	if(size1 != size2) err(1, "Invalid input");

	uint8_t currentByte1 = 0;
	uint8_t currentByte2 = 0;
	uint16_t index = 0;

	while((read(readingFd1, &currentByte1, sizeof(uint8_t)) > 0) 
		&& (read(readingFd2, &currentByte2, sizeof(uint8_t)) > 0))
	{

		if(currentByte1 == currentByte2) { index += 1; continue; }
		
		helper currentHelper;
		currentHelper.offset = index;
		currentHelper.original = currentByte1;
		currentHelper.new = currentByte2;
		
		if(write(writingFd, &currentHelper, sizeof(helper)) < 0) err(1, "Write error");
		index += 1;

	}

	close(readingFd1);
	close(readingFd2);
	close(writingFd);
	return 0;

}

