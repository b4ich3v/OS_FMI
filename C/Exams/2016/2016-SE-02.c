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

int main(void)
{

	while(true)
	{

		if(write(1, "> ", strlen("> "))<0) err(1, "Write error");

		char buffer[1024] = {0};
		char currentSymbol = 'a';
		int currentIndex = 0;

		while(read(0, &currentSymbol, sizeof(char)) > 0)
		{

			if(currentSymbol == '\n') break;

			buffer[currentIndex] = currentSymbol;
			currentIndex += 1;

		}


		buffer[currentIndex] = '\0';
		if(!strcmp(buffer, "exit")) break;

		pid_t currentPid = fork();
		if(currentPid < 0) err(1, "Fork error");
		if(currentPid == 0)
		{

			execlp(buffer, buffer, (char*) NULL);
			err(1, "Execlp error");


		}

		int status = 0;
		if(wait(&status) <0) err(1, "Wait error");
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Status error");

	}

	return 0;

}

