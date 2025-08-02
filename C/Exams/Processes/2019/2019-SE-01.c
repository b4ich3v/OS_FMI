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
#include <time.h>

int main(int argc, char* argv[])
{

	if(argc < 2) errx(1, "Wrong number of arguments");

	int tolerance = strtol(argv[1], NULL, 10);
	if (tolerance < 1 || tolerance > 9) errx(1, "Tolerance error");

	int countOfArguments = argc - 2;
	int index = 2;

	char** arguments = malloc(sizeof(char*) * countOfArguments);
	if(countOfArguments != 0 && arguments == NULL) err(1, "NULL error");

	for(int i = 0; i < countOfArguments; i++)
	{

		arguments[i] = malloc(sizeof(char) * strlen(argv[index]) + 1);

		for(int j = 0; j < (int)strlen(argv[index]); j++)
		{

			arguments[i][j] = argv[index][j];

		}

		arguments[i][strlen(argv[index])] = '\0';
		index += 1;

	}

	int writingFd = open("run.log", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	bool wasLastBad = false;
	bool isCurrentBad = false;

	while(true)
	{

		int sharedData[2];
		if(pipe(sharedData)<0) err(1, "Pipe error");

		pid_t pid = fork();
		if(pid < 0) err(1, "Fork error");
		if(pid == 0)
		{

			close(sharedData[0]);
			time_t start = time(NULL);
			if(write(sharedData[1], &start, sizeof(time_t)) < 0) err(1, "Error");
			close(sharedData[1]);

			execvp(arguments[0], arguments);
			err(1, "Execlp error");

		}

		close(sharedData[1]);
		time_t start = 0;
		if(read(sharedData[0], &start, sizeof(time_t)) < 0) err(1, "Read error");
		time_t end = time(NULL);
		close(sharedData[0]);

		int status = 0;
		if(wait(&status) <0) err(1, "Wait error");

		int exitCode;
		if (WIFEXITED(status)) exitCode = WEXITSTATUS(status);
		else if (WIFSIGNALED(status)) exitCode = 129;
		else exitCode = 1;

		char buffer[1024];
		if(snprintf(buffer, sizeof(buffer), "%ld %ld %d\n", start, end, exitCode) < 0) err(1, "Snprintf error");
		if(write(writingFd, buffer, strlen(buffer)) < 0) err(1, "Write error");

		if(exitCode != 0 && tolerance > (int)(end - start)) isCurrentBad = true;
		if(isCurrentBad && wasLastBad) break;
		wasLastBad = isCurrentBad;
		
	}

	for(int i = 0; i < countOfArguments; i++)
	{

		free(arguments[i]);

	}

	free(arguments);
	close(writingFd);
	
	return 0;

}
