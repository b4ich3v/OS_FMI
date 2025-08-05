#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef struct header
{
	uint16_t magic;
	uint16_t fileType;
	uint32_t count;
} header;

typedef struct pair
{
	uint16_t position;
	uint16_t value;

} pair;

int main(int argc, char* argv[])
{

	if(argc != 4) errx(1, "Wrong number of arguments");

	int listFd = open(argv[1], O_RDONLY);
	if(listFd < 0) err(1, "Open error");
	
	int dataFd = open(argv[2], O_RDONLY);
	if(dataFd < 0) err(1, "Open error");

	int writingFd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	header h;

	if(read(listFd, &h, sizeof(header)) < 0) err(1, "Read error");
	if(h.magic != 0x5A4D || h.fileType != 1) err(1, "Invalid input");

	if(read(dataFd, &h, sizeof(header)) < 0) err(1, "Read error");
	if(h.magic != 0x5A4D || h.fileType != 2) err(1, "Invalid input");
	int size2 = h.count;

	struct stat fileStat;
	if(stat(argv[1], &fileStat) < 0) err(1, "Stat error");
	
	h.magic = 0x5A4D;
	h.fileType = 3;
	h.count = fileStat.st_size / sizeof(pair);

	if(write(writingFd, &h, sizeof(header)) < 0) err(1, "Write error");

	pair currentPair;

	while(read(listFd, &currentPair, sizeof(pair)) > 0)
	{

		if (currentPair.position >= size2) err(1, "Invalid position");
		if(lseek(dataFd, currentPair.position * sizeof(uint32_t), SEEK_SET) < 0) err(1, "Lseek error");

		uint32_t currentNumber = 0;
		if(read(dataFd, &currentNumber, sizeof(uint32_t)) < 0) err(1, "Read error");

		uint64_t resultNumber = (uint64_t)currentNumber;
		if(lseek(writingFd, currentPair.value * sizeof(uint64_t), SEEK_SET) < 0) err(1, "Lseek error");
		if(write(writingFd, &resultNumber, sizeof(uint64_t)) < 0) err(1, "Write error");

	}

	close(dataFd);
	close(listFd);
	close(writingFd);
	return 0;

}
