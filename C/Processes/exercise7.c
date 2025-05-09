#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, const char* argv[])
{

    if(argc != 4)
    {

        errx(1, "Error\n");

    }

    for (int i = 1; i < 4; i++)
    {
        
        pid_t currentPid = fork();

        if(currentPid == 0)
        {

            execlp(argv[i], argv[i], (char*) NULL);
            err(1, "Error\n");

        }else
        {

            int waitStatus;
            waitpid(currentPid, &waitStatus, 0);

        }

    }

    exit(0);

}
