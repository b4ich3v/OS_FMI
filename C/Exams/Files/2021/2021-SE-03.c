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

char toUpper(char ch)
{

	if(!(ch >= 'a' && ch <= 'z')) return ch;
	return ch - 'a' + 'A';

}
int main(int argc, char* argv[])
{

	if(argc != 3) errx(1, "Wrong number of arguments");

	int readingFd = open(argv[1], O_RDONLY);
	if(readingFd < 0) err(1, "Open error");
	
	int writingFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");
	
	uint16_t currentNumber = 0;
	struct stat fileStat;
	if(stat(argv[1], &fileStat) < 0) err(1, "Stat error");
	int size = fileStat.st_size / sizeof(uint16_t);
	int index = 0;

	uint16_t* array = malloc(sizeof(uint16_t) * size);
	if(array == NULL) err(1, "Null error");

	while(read(readingFd, &currentNumber, sizeof(currentNumber)) > 0)
	{

		array[index] = currentNumber;
		index += 1;

	}

	char buffer[1024] = {0};
	char argBuffer[1024] = {0};

	for(int i = 0; i < (int)strlen(argv[1]); i++) argBuffer[i] = toUpper(argv[1][i]);
	argBuffer[(int)strlen(argv[1])] = '_';
	argBuffer[(int)strlen(argv[1]) + 1] = 'H';
	argBuffer[(int)strlen(argv[1]) + 2] = '\0';

	if(snprintf(buffer, sizeof(buffer), "#ifndef %s\n#define %s\n", argBuffer, argBuffer) < 0) err(1, "Snprintf error");
	if(write(writingFd, buffer, strlen(buffer)) < 0) err(1, "Write error");	
	if(snprintf(buffer, sizeof(buffer), "const uint16_t array[] = {") < 0) err(1, "Snprintf error");
	if(write(writingFd, buffer, strlen(buffer)) < 0) err(1, "Write error");

	for(int i = 0; i < size; i++)
	{

		char numberBuffer[100] = {0};

		if(i != size - 1)
		{		

			if(snprintf(numberBuffer, sizeof(numberBuffer), 
				"%d,", array[i]) < 0) err(1, "Snprintf error");

		}
		else
		{

			if(snprintf(numberBuffer, sizeof(numberBuffer), 
				"%d", array[i]) < 0) err(1, "Snprintf error");

		}

		if(write(writingFd, numberBuffer, strlen(numberBuffer)) < 0) err(1, "Write error");

	}

	free(array);

	if(snprintf(buffer, sizeof(buffer), "};\n") < 0) err(1, "Snprintf error");
	if(write(writingFd, buffer, strlen(buffer)) < 0) err(1, "Snprintf error");
	if(snprintf(buffer, sizeof(buffer), "#endif\n") < 0) err(1, "Snprintf error");
	if(write(writingFd, buffer, strlen(buffer)) < 0) err(1, "Write error");

	close(readingFd);
	close(writingFd);
	return 0;

}
