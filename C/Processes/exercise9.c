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

    pid_t pid1 = fork();
    if(pid1 < 0) err(1, "Error");

    if(pid1 == 0)
    {

        int fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        const char* buffer = "foo\n";

        if(write(fd, buffer, sizeof("foo\n")) < 0)
        {

            close(fd);
            err(1, "Error");

        }
        
        close(fd);
        exit(0);
        
    }

    int status = 0;
    waitpid(pid1, &status, 0);
    if(!WIFEXITED(status)) errx(1, "Error");

    pid_t pid2 = fork();
    if(pid2 < 0) err(1, "Error");

    if(pid2 == 0)
    {

        int fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, 0644);
        const char* buffer = "bar\n";

        if(write(fd, buffer, sizeof("bar\n")) < 0)
        {

            close(fd);
            err(1, "Error");

        }
        
        close(fd);
        exit(0);

    }
    
    waitpid(pid2, &status, 0);
    if(!WIFEXITED(status)) errx(1, "Error");
    exit(0);

}
