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

int main(int argc, char* argv[])
{
	
	bool hasNumbers = false;
	if(!strcmp(argv[1], "-n")) hasNumbers = true;

	int rowIndex = 1;

	for (int i = 1; i < argc; i++)
	{

		if(hasNumbers && i == 1) i += 1;
		const char* currentArg = argv[i];

		if(strcmp(currentArg, "-"))
		{

			int readingFd = open(currentArg, O_RDONLY);
			if(readingFd < 0) err(1, "Open error");

			char currentByte = 'a';
			char buffer[4096] = {0};
			int index = 0;

			while(read(readingFd, &currentByte, sizeof(char)) > 0)
			{

				buffer[index] = currentByte;
				index += 1;

			}

			buffer[index] = '\0';

			if(hasNumbers)
			{

				char currentLine[1024] = {0};
				int indexForLine = 0;

				for (int i = 0; i < index; i++)
				{
				
					if(buffer[i] == '\n')
					{

						char result[1024] = {0};

						if(snprintf(result, sizeof(result), "%d %s\n", rowIndex, currentLine) < 0) err(1, "Snprintf error");
						if(write(1, result, strlen(result)) < 0) err(1, "Write error");
						rowIndex += 1;
						indexForLine = 0;

					}
					else
					{

						currentLine[indexForLine] = buffer[i];
						indexForLine += 1;

					}

				}

			}
			else
			{

				if(write(1, buffer, strlen(buffer)) < 0) err(1, "Write error");

			}

			close(readingFd);

		}
		else
		{

			char currentByte = 'a';
			char buffer[1024] = {0};
			int index = 0;

			while(read(0, &currentByte, sizeof(char)) > 0)
			{

				if(currentByte == '\n') break;
				buffer[index] = currentByte;
				index += 1;

			}

			buffer[index] = '\0';

			if(hasNumbers)
			{

				char result[1024] = {0};
				if(snprintf(result, sizeof(result), "%d %s\n", i - 1, buffer) < 0) err(1, "Snprintf error");
				if(write(1, result, strlen(result)) < 0) err(1, "Write error");

			}
			else
			{

				if(write(1, buffer, strlen(buffer)) < 0) err(1, "Write error");

			}

		}

	}
	
	return 0;

}
