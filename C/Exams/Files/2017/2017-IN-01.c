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

typedef struct tuple
{
	uint16_t offset;
	uint8_t len;
	uint8_t dummy;
} tuple;

int main(int argc, char* argv[])
{
	
	if(argc != 5) errx(1, "Wrong number of argumensts");

	struct stat fileStat;
	int readingDat = open(argv[1], O_RDONLY);
	if(readingDat < 0) err(1, "Open error");
	if(stat(argv[1], &fileStat) < 0) err(1, "Stat error");
	int datSize = fileStat.st_size;

	int readingIdx = open(argv[2], O_RDONLY);
	if(readingIdx < 0) err(1, "Open error");
	if(stat(argv[2], &fileStat) < 0) err(1, "Stat error");
	int idxSize = fileStat.st_size;
	if(idxSize % sizeof(tuple) != 0) err(1, "Invalid input");

	int writingDat = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingDat < 0) err(1, "Open error");

	int writingIdx = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingIdx < 0) err(1, "Open error");
	
	tuple currentTuple;
	uint16_t index = 0;

	while(read(readingIdx, &currentTuple, sizeof(tuple)) > 0)
	{

		if((int)(currentTuple.offset * sizeof(char) + currentTuple.len) > datSize) err(1, "Invalid data");
		if(lseek(readingDat, currentTuple.offset * sizeof(char), SEEK_SET) < 0) err(1, "Lseek error");

		char currentByte = 'a';
		bool hasCandidate = false;
		char* buffer = malloc(sizeof(char) * currentTuple.len + 1);
		if(read(readingDat, buffer, currentTuple.len) < 0) err(1, "Read error");
		buffer[currentTuple.len] = '\0';

		if(buffer[0] >= 'A' && buffer[0] <= 'Z') hasCandidate = true;

		if(hasCandidate)
		{

			tuple newTuple;
			newTuple.offset = index;
			newTuple.len = currentTuple.len;
			newTuple.dummy = currentTuple.dummy;

			if(write(writingIdx, &newTuple, sizeof(tuple)) < 0) err(1, "Write error");
			if(write(writingDat, buffer, currentTuple.len) < 0) err(1, "Write error");
			index += currentTuple.len;

		}

	}

	close(readingDat);
	close(readingIdx);
	close(writingDat);
	close(writingIdx);
	return 0;

}

