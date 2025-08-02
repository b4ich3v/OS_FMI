#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[])
{

	if(argc > 2) errx(1, "Wrong number of arguments");

	const char* command = NULL;
	if(argc == 2) command = argv[1];
	else command = "echo";

	char buffer[1024] = {0};
	int readBytes = 0;
	if((readBytes = read(0, buffer, sizeof(buffer))) < 0) err(1, "Read error");

	char argument1[5]; argument1[0] = '\0';
	char argument2[5]; argument2[0] = '\0';
	int countOfCurrentCommand = 0;
	int prevICounter = 0;

	for(int i = 0; i < readBytes; i++)
	{

		if(countOfCurrentCommand > 2) err(1, "Extra count of arguments");

		if(buffer[i] == ' ' || buffer[i] == '\n')
		{

			if(prevICounter > 4) err(1, "Too long argument");
			i -= prevICounter;
			
			for(int j = 0; j < prevICounter ; j++)
			{

				if(countOfCurrentCommand == 0) argument1[j] = buffer[i];
				else if(countOfCurrentCommand == 1) argument2[j] = buffer[i];

			}
			
			if(countOfCurrentCommand == 0) argument1[prevICounter] = '\0';
			else if(countOfCurrentCommand == 1) argument2[prevICounter] = '\0';

			i += prevICounter;
			prevICounter = 0;
			countOfCurrentCommand += 1;

		}
		else
		{
			
			prevICounter += 1;
			
		}

	}

	pid_t pid = fork();
	if(pid <0) err(1, "Fork error");
	if(pid ==0)
	{

		if(argument1[0] != '\0' &&  argument2[0] != '\0') 
		{

			execlp(command, command, argument1, argument2, (char*) NULL);

		}
		else if(argument1[0] != '\0' &&  argument2[0] == '\0')
		{

			execlp(command, command, argument1, (char*) NULL);

		}
		else if(argument1[0] == '\0' &&  argument2[0] == '\0')
		{

			err(1, "No arguments");

		}

		err(1, "Execlp error");

	}

	int status = 0;
	if(wait(&status) < 0) err(1, "Wait error");
	if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Status error");

	return 0;

}

