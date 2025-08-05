#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{

	if(argc != 3) errx(1, "Wrong number of arguments");

	int readingFd = open(argv[1], O_RDONLY);
	if(readingFd < 0) err(1, "Open error"); 

	int writingFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	uint16_t currentNumber = 0;

	while(read(readingFd, &currentNumber, sizeof(uint16_t)) > 0)
	{

		uint8_t currentByte = 0;

		for(int i = 7; i >= 0; i--)
		{

			uint8_t currentPair = 0;
			currentPair = (currentNumber >> (2 * i)) & 3;

			if(currentPair == 2)
			{

				currentByte |= (1 << i);

			}

		}	

		if(write(writingFd, &currentNumber, sizeof(uint8_t)) < 0) err(1, "Write error");

	}

	close(readingFd);
	close(writingFd);
	return 0;

}

