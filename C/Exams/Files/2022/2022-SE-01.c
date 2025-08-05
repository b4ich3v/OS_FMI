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

typedef struct dataHeader
{
	uint32_t magic;
	uint32_t count;
} dataHeader; 

typedef struct comparatorHeader
{
	uint32_t magic1;
	uint16_t magic2;
	uint16_t reserved;
	uint64_t count;
} comparatorHeader;

typedef struct comparatorElement
{
	uint16_t type;
	uint16_t reserved[3]; 
	uint32_t offsets[2];
} comparatorElement;

int main(int argc, char* argv[])
{

	if(argc != 3) errx(1, "Wrong number of arguments");

	int dataFd = open(argv[1], O_RDWR);
	if(dataFd < 0) err(1, "Open error");

	int comparatorFd = open(argv[2], O_RDONLY);
	if(comparatorFd < 0) err(1, "Open error");

	comparatorHeader header1;
	if(read(comparatorFd, &header1, sizeof(comparatorHeader)) < 0) err(1, "Read error");
	if(header1.magic1 != 0xAFBC7A37 || header1.magic2 != 0x1C27) err(1, "Invalid input");

	dataHeader header2;
	if(read(dataFd, &header2, sizeof(dataHeader)) < 0) err(1, "Read error");
	if(header2.magic != 0x21796F4A) err(1, "Invalid input");
	uint32_t countOfElementsInData = header2.count;

	comparatorElement current;

	while(read(comparatorFd, &current, sizeof(comparatorElement)) > 0)
	{

		for(int i = 0; i < 3; i++)
		{
			
			if(current.reserved[i] != 0) err(1, "Invalid input");

		}
		
		if(current.offsets[0] >= countOfElementsInData || current.offsets[1] >= countOfElementsInData) err(1, "Invalid input");

		uint64_t left = 0;
		uint64_t right = 0;
		bool isGreater = (current.type == 1);

		if(lseek(dataFd, current.offsets[0] * sizeof(uint64_t), SEEK_SET) < 0) err(1, "Lseek error");
		if(read(dataFd, &left, sizeof(uint64_t)) < 0 ) err(1, "Read error");
		if(lseek(dataFd, current.offsets[1] * sizeof(uint64_t), SEEK_SET) < 0) err(1, "Lseek error");
		if(read(dataFd, &right, sizeof(uint64_t)) < 0 ) err(1, "Read error");

		if((isGreater && left < right) || (!isGreater && left > right))
		{

			if(lseek(dataFd, current.offsets[0] * sizeof(uint64_t), SEEK_SET) < 0) err(1, "Lseek error");
			if(write(dataFd, &right, sizeof(uint64_t)) < 0 ) err(1, "Write error");
			if(lseek(dataFd, current.offsets[1] * sizeof(uint64_t), SEEK_SET) < 0) err(1, "Lseek error");
			if(write(dataFd, &left, sizeof(uint64_t)) < 0 ) err(1, "Write error");

		}
		
	}

	close(dataFd);
	close(comparatorFd);
	return 0;

}
