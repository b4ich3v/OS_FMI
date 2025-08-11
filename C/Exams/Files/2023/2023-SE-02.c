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

int main(int argc, char* argv[])
{
	
	if(argc != 3) errx(1, "Wrong number of arguments");

	const char* targetWord = argv[1];
	int readingFd = open(argv[2], O_RDONLY);
	if(readingFd < 0) err(1, "Read error");

	int left = 0;
	int right = 0;
	char currentByte = 0;
	int currentIndex = 0;
	int counterForReadBytes = 0;

	while(read(readingFd, &currentByte, sizeof(char)) > 0)
	{

		if(currentByte == 0) right += 1;
		
	}
	
	right -= 1;

	int* array = malloc(right * sizeof(int));
	if(array == NULL) err(1, "Null error");
	if(lseek(readingFd, 0, SEEK_SET) < 0) err(1, "Lseek error");

	while(read(readingFd, &currentByte, sizeof(char)) > 0)
	{

		if(currentByte == 0) 
		{
		
			array[currentIndex] = counterForReadBytes;
			currentIndex += 1;

		}
		
		counterForReadBytes += 1;
		
	}

	if(lseek(readingFd, 0, SEEK_SET) < 0) err(1, "Lseek error");

	while(left <= right)
	{
		
		int middle = left + (right - left) / 2;
		if(lseek(readingFd, array[middle] * sizeof(char) + 1, SEEK_SET) < 0) err(1, "Lseek error");

		char buffer[64] = {0};
		currentIndex = 0;

		while(read(readingFd, &currentByte, sizeof(char)) > 0)
		{

			if(currentByte == '\n') break;
			buffer[currentIndex] = currentByte;
			currentIndex += 1;

		}

		buffer[currentIndex] = '\0';
		
		if(strcmp(targetWord, buffer) > 0) left = middle + 1;
		else if(strcmp(targetWord, buffer) < 0) right = middle - 1;
		else 
		{

			while(read(readingFd, &currentByte, sizeof(char)) > 0)
			{

				if(currentByte == 0) break;
				if(write(1, &currentByte, sizeof(char)) < 0) err(1, "Write error");

			}

			close(readingFd);
			free(array);
			return 0;	

		}

	}

	if(write(1, "Not found", strlen("Not found")) < 0) err(1, "Write error");
	close(readingFd);
	free(array);
	return -1;

}
