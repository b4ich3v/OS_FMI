#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{

	if(argc != 4) errx(1, "Wrong number of arguments");

	int readingFd1 = open(argv[1], O_RDONLY);
	if(readingFd1 < 0) err(1, "Open error");

	int readingFd2 = open(argv[2], O_RDONLY);
	if(readingFd2 < 0) err(1, "Open error");

	int writingFd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	uint8_t currentByte = 0;

	while(read(readingFd1, &currentByte, sizeof(uint8_t)) > 0)
	{

		for(int i = 7; i >= 0; i--)
		{

			uint16_t currentNumber = 0;
			if(read(readingFd2, &currentNumber, sizeof(uint16_t)) < 0) err(1, "Read error");

			if(currentByte & (1 << i))
			{

				if(write(writingFd, &currentNumber, sizeof(uint16_t)) < 0) err(1, "Write error");

			}

		}

	}

	close(readingFd1);
	close(readingFd2);
	return 0;

}

