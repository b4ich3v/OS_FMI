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

    int fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, 0644);
    if(fd < 0) err(1, "Error");

    int writtenBytes = write(fd, "fo", strlen("fo"));
    if(writtenBytes < 0) {err(1, "Error"); close(fd);}

    pid_t pid = fork();
    if(pid < 0) {err(1, "Error"); close(fd);}

    if(pid == 0)
    {

        writtenBytes = write(fd, "bar\n", strlen("bar\n"));
        if(writtenBytes < 0) {err(1, "Error"); close(fd);}
        exit(0);

    }

    int status = 0;
    wait(&status);

    writtenBytes = write(fd, "o\n", strlen("o\n"));
    if(writtenBytes < 0) {err(1, "Error"); close(fd);}

    close(fd);
    exit(0);

}
