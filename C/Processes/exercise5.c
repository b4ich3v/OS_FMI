#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <sys/wait.h>

int main(void)
{

    pid_t childPid = fork();

    if(childPid == 0)
    {

        write(1, "Children\n", strlen("Children\n"));
        exit(0);

    }

    int waitStatus;
    if(waitpid(childPid, &waitStatus, 0) < 0)
    {

        err(1, "Error");

    }
    
    write(1, "Father\n", strlen("Father\n"));
    exit(0);

}
