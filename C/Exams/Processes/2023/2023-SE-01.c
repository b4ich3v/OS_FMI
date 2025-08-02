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

	if(argc != 2) errx(1, "Wrong number of arguments");

	char template[] = "tempFileXXXXXX";
	int fd = mkstemp(template);
	if(fd < 0) err(1, "Open error");

	pid_t pid = fork();
	if(pid < 0) err(1, "Fork error");
	if(pid == 0)
	{

		if(dup2(fd, 1) < 0) err(1, "Dup2 error");
		close(fd);

		execlp("find", "find", argv[1], "-type", "f", "-iname", "*[^.hash]", (char*) NULL);
		err(1, "Execlp error");

	}

	int status = 0;
	if(wait(&status) < 0) err(1, "Wait error");
	if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Status error");

	if(lseek(fd, 0, SEEK_SET) < 0) err(1, "Lseek error");
	int countOfProcesses = 0;
	char currentByte = 'a';

	while(read(fd, &currentByte, sizeof(char)) > 0)
	{

		if(currentByte == '\n') countOfProcesses += 1;

	}

	if(lseek(fd, 0, SEEK_SET) < 0) err(1, "Lseek error");

	int** sharedData = malloc(sizeof(int*) * countOfProcesses);
	if(sharedData == NULL) err(1, "Null error");
	for(int i = 0; i < countOfProcesses; i++) sharedData[i] = malloc(sizeof(int) * 2);

	for(int i = 0; i < countOfProcesses; i++) 
	{

		if(pipe(sharedData[i]) < 0) err(1, "Pipe error");		

		pid_t currentPid = fork();
		if(currentPid < 0) err(1, "Fork error");
		if(currentPid == 0)
		{

			close(sharedData[i][1]);
			char buffer[1024] = {0};
			int readBytes = 0;

			if((readBytes = read(sharedData[i][0], buffer, sizeof(buffer))) < 0) err(1, "Read error");
			buffer[readBytes] = '\0';
			close(sharedData[i][0]);

			char result[1024] = {0};
			if(snprintf(result, sizeof(result), "%s.hash", buffer) < 0) err(1, "Snprintf error");
			int writingFd = open(result, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if(writingFd < 0) err(1, "Open error");
			if(dup2(writingFd, 1) < 0) err(1, "Dup2 error");

			execlp("md5sum", "md5sum", buffer, (char*) NULL);
			err(1, "Execlp error");

		}

		close(sharedData[i][0]);

	}

	for(int i = 0; i < countOfProcesses; i++)
	{

		char buffer[1024] = {0};
		int currentIndex = 0;
		char currentByte = 'a';

		while(read(fd, &currentByte, sizeof(char)) > 0)
		{

			if(currentByte == '\n') break;
			buffer[currentIndex] = currentByte;
			currentIndex += 1;
		
		}

		buffer[currentIndex] = '\0';
		if(write(sharedData[i][1], buffer, strlen(buffer)) < 0) err(1, "Write error");

		close(sharedData[i][1]);

	}

	for (int i = 0; i < countOfProcesses; i++)
	{
		
		int status = 0;
		if(wait(&status) < 0) err(1, "Wait error");
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Status error");

	}
	

	for(int i = 0; i < countOfProcesses; i++)
	{

		free(sharedData[i]);

	}

	free(sharedData);
	unlink(template);
	close(fd);
	
	return 0;

}
