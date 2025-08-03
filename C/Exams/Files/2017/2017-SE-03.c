#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>
#include <stdlib.h>

typedef struct helper
{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
} helper;

int main(int argc, char* argv[])
{

	if(argc != 4) errx(1, "Wrong number of arguments");

	struct stat fileStat;

	int patchFd = open(argv[1], O_RDONLY);
	if(patchFd < 0) err(1, "Open error");
	if(stat(argv[1], &fileStat) <0) err(1, "Stat error");
	if(fileStat.st_size % sizeof(helper) != 0) err(1, "Invalid input");
	int countOfHelpers = fileStat.st_size / sizeof(helper);

	int readingFd = open(argv[2], O_RDONLY);
	if(readingFd < 0) err(1, "Open error");
	if(stat(argv[2], &fileStat) <0) err(1, "Stat error");
	int size = fileStat.st_size;

	int writingFd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	helper* allElements = malloc(sizeof(helper) * countOfHelpers);
	if(allElements == NULL) err(1, "Null error");
	int index = 0;

	while(read(patchFd, &allElements[index], sizeof(helper)) > 0)
	{

		if((int)(allElements[index].offset * sizeof(uint8_t) + 1) > size) err(1, "Invalid input");
		index += 1;
		
	}

	index = 0;
	uint8_t currentByte = 0;

	while(read(readingFd, &currentByte, sizeof(uint8_t)) > 0)
	{

		int targetIndex = -1;

		for(int i = 0; i < countOfHelpers; i++)
		{

			if(allElements[i].offset == index)
			{

				targetIndex = i;
				break;

			}

		}
	
		if(targetIndex != -1)
		{

			if(allElements[targetIndex].original == currentByte)
			{

				if(write(writingFd, &allElements[targetIndex].new, sizeof(uint8_t)) < 0) err(1, "Write error");

			}
			else
			{

				err(1, "Error");


			}

		}
		else
		{

			if(write(writingFd, &currentByte, sizeof(uint8_t)) < 0) err(1, "Write error");

		}

		index += 1;	

	}

	free(allElements);
	close(patchFd);
	close(readingFd);
	close(writingFd);
	return 0;

}

