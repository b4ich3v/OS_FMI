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

    int countOfCommands = argc;
    int countOfSuccess = 0;
    int countOfLost = 0;

    int storedFd = dup(STDOUT_FILENO);
    int trashFd = open("/dev/null", O_WRONLY);
    dup2(trashFd, STDOUT_FILENO);

    for (int i = 1; i < countOfCommands; i++)
    {
        
        pid_t currentPid = fork();

        if(currentPid == 0)
        {

            execlp(argv[i], argv[i], (char*) NULL);
            exit(0);

        }

    }
    
    int currentStatus = 0;

    for (int i = 0; i < countOfCommands - 1; i++)
    {
        
        wait(&currentStatus);

        if(WIFEXITED(currentStatus) && WEXITSTATUS(currentStatus)) countOfSuccess += 1;
        else countOfLost += 1;

    }
    
    dup2(storedFd, STDOUT_FILENO);
    printf("%d \n", countOfSuccess);
    printf("%d \n", countOfLost);
    exit(0);

}
