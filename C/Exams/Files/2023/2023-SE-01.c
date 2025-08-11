#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>

int main(int argc, char* argv[])
{

	if(argc != 3) errx(1, "Wrong number of arguments");

	int readingFd = open(argv[1], O_RDONLY);
	if(readingFd < 0) err(1, "Open error");

	int writingFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	uint8_t currentByte = 0;
	int counter = 0;
	struct stat fileStat;
	if(stat(argv[1], &fileStat) <0) err(1, "Stat error");
	int sizeOfFile = fileStat.st_size;

	while(read(readingFd, &currentByte, sizeof(uint8_t)) > 0)
	{

		counter += 1;

		if(currentByte == (uint8_t)0x55)
		{

			uint8_t result = 0;
			result ^= currentByte;
			uint8_t N = 0;
			if(read(readingFd, &N, sizeof(uint8_t))<0) err(1, "Read error");
			if(N - 2 + counter >= sizeOfFile) err(1, "Invalid input data");
			result ^= N;
			
			for(int i = 0; i < N - 3; i++)
			{

				counter += 1;
				if(read(readingFd, &currentByte, sizeof(uint8_t)) < 0) err(1, "Read error");
				result ^= currentByte;

			}
			
			uint8_t lastByte = 0;
			if(read(readingFd, &lastByte, sizeof(uint8_t)) <0) err(1, "Read error");

			if(lastByte == result)
			{

				if(lseek(readingFd, -N * sizeof(uint8_t), SEEK_CUR) < 0) err(1, "Lseek error");

				for(int i = 0; i < N; i++)
				{

					if(read(readingFd, &currentByte, sizeof(uint8_t))<0) err(1, "Read error");
					if(write(writingFd, &currentByte, sizeof(uint8_t))<0) err(1, "Write error");

				}

			}

		}

	}

	close(readingFd);
	close(writingFd);
	return 0;

}


