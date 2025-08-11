#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>
#include <string.h>

typedef struct header
{
	uint32_t magic;
	uint32_t packet_count;
	uint64_t original_size;
} header;

int main(int argc, char* argv[])
{

	if(argc != 3) errx(1, "Wrong number of arguments");

	int readingFd = open(argv[1], O_RDONLY);
	if(readingFd < 0) err(1, "Open error");

	int writingFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");
	
	header h;
	if(read(readingFd, &h, sizeof(header)) < 0) err(1, "Read error");
	if(h.magic != 0x21494D46) err(1, "Invalid input data");
	
	uint8_t currentByte = 0;
	int writtenBytes = 0;

	for(int i = 0; i < (int)h.packet_count; i++)
	{
		
		if(read(readingFd, &currentByte, sizeof(uint8_t)) < 0) err(1, "Read error");
		bool isFirstType = (1 << 7) & currentByte;
		int N = ((1 << 7) - 1) & currentByte;

		if(!isFirstType)
		{

			for(int j = 0; j < N + 1; j++)
			{

				if(read(readingFd, &currentByte, sizeof(uint8_t)) < 0) err(1, "Read error");
				if(write(writingFd, &currentByte, sizeof(uint8_t)) < 0) err(1, "Write error");	

			}

			writtenBytes += (N + 1);

		}
		else
		{

			uint8_t byteToRepeat;
			if(read(readingFd, &byteToRepeat, sizeof(uint8_t)) != 1) err(1, "Read error");

			for(int j = 0; j < N + 1; j++) 
			{

    			if(write(writingFd, &byteToRepeat, sizeof(uint8_t)) != 1) err(1, "Write error");

			}
			
			writtenBytes += (N + 1);

		}

	}	

	if(writtenBytes > (int)(h.original_size)) err(1, "Invalid data input");

	close(readingFd);
	close(writingFd);
	return 0;

}
