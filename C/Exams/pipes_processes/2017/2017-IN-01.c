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

    if(argc != 1) errx(1, "Error");
    
    char* commands[][10] = 
    {

        {"cat", "/etc/passwd", NULL},
        {"cut", "-d", ":", "-f", "7", NULL},
        {"uniq", "-c", NULL},
        {"sort", "-u", NULL},
        {"tr", "-s", " ", NULL},
        {"cut", "-d", " ", "-f", "3", NULL},
        {NULL}

    };

    int countOfCommands = 6;
    int prevFd = -1;
    int sharedData[2];

    for (int i = 0; i < countOfCommands; i++)
    {
        
        if(i != countOfCommands - 1)
        {

            if(pipe(sharedData) < 0) err(1, "Error");

        }

        pid_t currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {

            if (prevFd != -1) 
            {

                dup2(prevFd, STDIN_FILENO); 
                close(prevFd);

            }

            if (i != countOfCommands - 1) 
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
    
    int fictiveVariableStatus = 0;

    for (int i = 0; i < countOfCommands; i++)
    {
        
        wait(&fictiveVariableStatus);
        if(!WIFEXITED(fictiveVariableStatus) || WEXITSTATUS(fictiveVariableStatus) != 0) 
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
