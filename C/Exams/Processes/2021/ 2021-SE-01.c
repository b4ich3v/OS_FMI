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

int sharedData[2] = {0};
char result[1024] = {0};
int sharedStatus = 0;

char date[1024];
char username[1024];
char arguments[1024];

int main(int argc, char* argv[])
{
	
	int writingFd = open("foo.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	if(pipe(sharedData) < 0) err(1, "Pipe erorr");

	pid_t pid1 = fork();
	if(pid1 < 0) err(1, "Fork error");
	if(pid1 == 0)
	{

		close(sharedData[0]);
		if(dup2(sharedData[1], 1) < 0) err(1, "Dup2 error");
		close(sharedData[1]);
		
		execlp("date", "date", "+Y-%m-%d %H:%M:%S.%6N", (char*) NULL);
		err(1, "Execlp error");

	}

	close(sharedData[1]);
	int readBytes = 0;
	if((readBytes = read(sharedData[0], date, sizeof(date))) < 0) err(1, "Read error");
	date[readBytes] = '\0';
	close(sharedData[0]);

	if(wait(&sharedStatus) < 0) err(1, "Wait error");
	if(!WIFEXITED(sharedStatus) || WEXITSTATUS(sharedStatus) != 0) err(1, "Status error");

	if(pipe(sharedData) < 0) err(1, "Pipe error");

	pid_t pid2 = fork();
	if(pid2 < 0) err(1, "Fork error");
	if(pid2 == 0)
	{

		close(sharedData[0]);
		if(dup2(sharedData[1], 1) < 0) err(1, "Dup2 error");
		close(sharedData[1]);

		execlp("id", "id", "-nu", (char*) NULL);
		err(1, "Execlp error");

	}	

	close(sharedData[1]);
	if((readBytes = read(sharedData[0], username, sizeof(username))) < 0) err(1, "Read error");
	username[readBytes] = '\0';
	close(sharedData[0]);

	if(wait(&sharedStatus) < 0) err(1, "Wait error");
	if(!WIFEXITED(sharedStatus) || WEXITSTATUS(sharedStatus) != 0) err(1, "Status error");

	int index = 0;

	for(int i = 0; i < argc; i++)
	{

		for(int j = 0; j < (int)strlen(argv[i]); j++)
		{

			arguments[index] = argv[i][j];
			index += 1;	

		}
		
		arguments[index] = ' ';
		index += 1;

	}

	if(snprintf(result, sizeof(result), "%s %s %s\n", date, username, arguments) < 0) err(1, "Snprintf error");

	for (int i = 0; i < (int)strlen(result); i++)
	{
		
		if(result[i] == '\n' && i != (int)(strlen(result) - 1)) result[i] = ' ';
		
	}
	
	if(write(writingFd, result, strlen(result)) < 0) err(1, "Write error");
	close(writingFd);

	pid_t pid3 = fork();
	if(pid3 < 0) err(1, "Fork error");
	if(pid3 == 0)
	{

		execlp("echo", "echo", "passwd", "-l", username, (char*) NULL);
		err(1, "Execlp error");

	}

	if(wait(&sharedStatus) < 0) err(1, "Wait error");
	if(!WIFEXITED(sharedStatus) || WEXITSTATUS(sharedStatus) != 0) err(1, "Status error");

	pid_t pid4 = fork();
	if(pid4 < 0) err(1, "Fork error");
	if(pid4 == 0)
	{

		execlp("echo", "echo", "pkill", "-u", username, (char*) NULL);
		err(1, "Execlp error");

	}

	if(wait(&sharedStatus) < 0) err(1, "Wait error");
	if(!WIFEXITED(sharedStatus) || WEXITSTATUS(sharedStatus) != 0) err(1, "Status error");

	return 0;

}


