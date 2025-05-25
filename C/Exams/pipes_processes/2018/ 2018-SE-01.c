#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

    if(argc != 2) errx(1, "Error");
    const char* currentFileName = NULL;

    char* commands[][20] = 
    {

        {"find", argv[1], "-mindepth", "1", "-maxdepth", "1", "-type", "f", "-printf", "%T@ %p\n", NULL},
        {"sort", "-n", "-r", NULL},
        {"head", "-n", "1", NULL},
        {"cut", "-d", " ", "-f2", NULL},
        {NULL}

    };

    int countOfCommands = 4;
    int prevFd = -1;
    int sharedData[2];

    for (int i = 0; i < countOfCommands; i++)
    {
        
        if(i != countOfCommands - 1)
        {

            if(pipe(sharedData) < 0 ) err(1, "Error");

        }

        pid_t currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {
            
            if(prevFd != -1)
            {

                dup2(prevFd, STDIN_FILENO);
                close(prevFd);

            }

            if(i != countOfCommands - 1)
            {

                close(sharedData[0]);
                dup2(sharedData[1], STDOUT_FILENO);
                close(sharedData[1]);

            }

            execvp(commands[i][0], commands[i]);
            err(1, "Error");

        }

        if(prevFd != -1) close(prevFd);
        if(i != countOfCommands - 1)
        {

            close(sharedData[1]);
            prevFd = sharedData[0];

        }        

    }
     
    for (int i = 0; i < countOfCommands; i++)
    {
            
        int currentStatus = 0;
        wait(&currentStatus);

        if(!WIFEXITED(currentStatus) || WEXITSTATUS(currentStatus) != 0) 
        {

            close(sharedData[0]);
            close(sharedData[1]);
            err(1, "Error");

        }

    }

    close(sharedData[0]);
    close(sharedData[1]);
    exit(0);

}

