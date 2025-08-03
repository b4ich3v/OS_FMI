#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{

	for (int i = 1; i < argc; i++)
	{
		
		if(strcmp(argv[i], "-"))
		{

			int readingFd = open(argv[i], O_RDONLY);
			if(readingFd < 0) err(1, "Read error");

			struct stat fileStat;
			if(stat(argv[i], &fileStat) < 0) err(1, "Stat error");

			char* buffer = malloc(sizeof(char) * fileStat.st_size + 1);
			if(buffer == NULL) err(1, "Null error");
			if(read(readingFd, buffer, fileStat.st_size) < 0) err(1, "Read error");
			buffer[fileStat.st_size] = '\0';
			if(write(1, buffer, fileStat.st_size) < 0) err(1, "Write error");

			free(buffer);
			close(readingFd);

		}
		else
		{

			char buffer[4096] = {0};
			int index = 0;
			char currentByte = 'a';

			while(read(0, &currentByte, sizeof(char)) > 0)
			{

				if(currentByte == '\n') break;
				buffer[index] = currentByte;
				index += 1;

			}

			buffer[index] = '\0';
			if(write(1, buffer, index * sizeof(char)) < 0) err(1, "Write error");

		}

	}

	return 0;

}

