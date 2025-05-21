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

    if (argc != 2) errx(1, "Error");

    int fds[2];
    if (pipe(fds) == -1) err(1, "Error");

    pid_t pid1 = fork();
    if (pid1 < 0) err(1, "fork");

    if (pid1 == 0) 
    {
        
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        close(fds[1]);

        execlp("cat", "cat", argv[1], (char*) NULL);
        err(1, "Error");

    }

    pid_t pid2 = fork();
    if (pid2 < 0) err(1, "Error");

    if (pid2 == 0) 
    {
        
        close(fds[1]);
        dup2(fds[0], STDIN_FILENO);
        close(fds[0]);

        execlp("sed", "sed", "-E", "s/./& /g", (char*) NULL);
        err(1, "Error");

    }

    close(fds[0]);
    close(fds[1]);
    wait(NULL);
    wait(NULL);
    exit(0);
    
}
