#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{

    if(argc != 3) errx(1, "Error");

    pid_t pid1 = fork();
    if(pid1 < 0) err(1, "Error");
    
    if(pid1 == 0)
    {

        execlp(argv[1], argv[1], (char*) NULL);
        err(1, "Error");

    }

    pid_t pid2 = fork();
    if(pid2 < 0) err(1, "Error");

    if(pid2 == 0)
    {

        execlp(argv[2], argv[2], (char*) NULL);
        err(1, "Error");

    }

    int status = 0;

    pid_t firstDone = wait(&status);
    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) 
    {

        write(1, "-1", 2);
        err(1, "Error");

    }

    pid_t secondDone = wait(&status);
    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) 
    {

        write(1, "-1", 2);
        err(1, "Error");

    }

    char resultBuffer[100];
    int writtenBytes = snprintf(resultBuffer, sizeof(resultBuffer), "%d\n", firstDone);
    if(writtenBytes < 0 || writtenBytes >= sizeof(resultBuffer)) err(1, "Error");
    resultBuffer[writtenBytes] = '\0';

    write(1, resultBuffer, strlen(resultBuffer));
    exit(0);

}
