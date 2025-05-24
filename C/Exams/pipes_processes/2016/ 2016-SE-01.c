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

    if(argc != 2) errx(1, "Error");

    int fd = open(argv[1], O_RDONLY);
    if(fd < 0) errx(1, "Error");

    int sharedData[2];
    if (pipe(sharedData) < 0) { close(fd); errx(1, "Error"); } 

    pid_t pid1 = fork();
    if(pid1 < 0) err(1, "Error");

    if(pid1 == 0)
    {

        dup2(fd, STDIN_FILENO);
        dup2(sharedData[1], STDOUT_FILENO);

        close(fd);
        close(sharedData[0]);
        close(sharedData[1]);

        execlp("sort", "sort", (char*) NULL);
        err(1, "Error");

    }

    pid_t pid2 = fork();
    if(pid2 < 0) err(1, "Error");

    if(pid2 == 0)
    {

        dup2(sharedData[0], STDIN_FILENO);

        close(fd);
        close(sharedData[0]);
        close(sharedData[1]);

        execlp("cat", "cat", (char*) NULL);
        err(1, "Error");

    }

    int status = 0;
    wait(&status);
    if(WIFEXITED(status) || WEXITSTATUS(status) != 0) 
    {

        close(sharedData[0]);
        close(sharedData[1]);
        close(fd);
        err(1, "Error");

    }

    wait(&status);
    if(WIFEXITED(status) || WEXITSTATUS(status) != 0) 
    {

        close(sharedData[0]);
        close(sharedData[1]);
        close(fd);
        err(1, "Error");

    }

    close(sharedData[0]);
    close(sharedData[1]);
    close(fd);
    exit(0);

}
