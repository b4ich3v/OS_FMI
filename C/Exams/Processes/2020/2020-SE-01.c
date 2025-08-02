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

/* int main(int artg, char* argv[])
{

	if(argc != 2) errx(1, "Wrong number of arguments");
	int readingFd = open("MKFIFO", O_RDONLY);
	if(readingFd < 0) err(1, "Read error");

	if(dup2(readingFd, 0) < 0) err(1, "Error");
	close(readingFd);

	execlp("sort", "sort", argv[1], (char*) NULL);
	err(1, "Exexlp error");

	return 0;

} */

int main(int argc, char* argv[])
{

	if(argc != 2) errx(1, "Wrong number of arguments");
	if(mkfifo("MKFIFO") < 0) err(1, "Error");
	
	int writingFd = open("MKFIFO", O_WRONLY);
	if(writingFd < 0) err(1, "Error");

	if(dup2(writingFd, 1) < 0) err(1, "Dup2 error");
	close(writingFd);

	execlp("cat", "cat", argv[1], (char*) NULL);
	err(1, "Execlp error");

	return 0;

}
