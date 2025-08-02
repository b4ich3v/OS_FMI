#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{

	if(argc != 2) errx(1, "Wrong number of arguments");

	int sharedData[2];
	if(pipe(sharedData)<0) err(1, "Pipe error");

	pid_t pid1 = fork();
	if(pid1 < 0) err(1, "Fork error");
	if(pid1 == 0)
	{

		close(sharedData[0]);
		if(dup2(sharedData[1], 1)<0) err(1, "Dup2 error");
		close(sharedData[1]);

		execlp("cat", "cat", argv[1], (char*) NULL);
		err(1, "Execlp error");

	}

	pid_t pid2 = fork();
	if(pid2 < 0) err(1, "Fork error");
	if(pid2 == 0)
	{

		close(sharedData[1]);
		if(dup2(sharedData[0], 0)<0) err(1, "Dup2 error");
		close(sharedData[0]);

		execlp("sort", "sort", (char*) NULL);
		err(1, "Execlp error");

	}

	close(sharedData[0]);
	close(sharedData[1]);

	int sharedStatus = 0;
	if(wait(&sharedStatus)<0) err(1, "Wait error");
	if(!WIFEXITED(sharedStatus) || WEXITSTATUS(sharedStatus) != 0) err(1, "Status error");
	if(wait(&sharedStatus)<0) err(1, "Wait error");
	if(!WIFEXITED(sharedStatus) || WEXITSTATUS(sharedStatus) != 0) err(1, "Status error");

	return 0;

}

