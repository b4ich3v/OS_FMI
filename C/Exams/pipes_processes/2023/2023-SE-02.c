#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    bool foundSomeError = false;
    bool everythingIsOk = false;
    const char* target = "found it!\n";

    int sharedData[100][2];
    int arrayOfPids[100];
    bool finished[100] = {false};

    for (int i = 0; i < argc - 1; i++)
    {
        
        if(pipe(sharedData[i]) < 0) err(1, "Error");

        pid_t currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {

            close(sharedData[i][0]);
            dup2(sharedData[i][1], STDOUT_FILENO);
            close(sharedData[i][1]);

            execlp(argv[i + 1], argv[i + 1], (char*) NULL);
            err(1, "Error");

        }

        arrayOfPids[i] = currentPid;
        close(sharedData[i][1]);

    }
    
    for (int i = 0; i < argc - 1; i++)
    {
        
        char buffer[1024];
        int bytesRead = read(sharedData[i][0], buffer, sizeof(buffer) - 1);
        if (bytesRead < 0) err(1, "Error");
        buffer[bytesRead] = '\0';

        close(sharedData[i][0]);

        int currentStatus = 0;
        waitpid(arrayOfPids[i], &currentStatus, 0);
        finished[i] = true;

        if(!WIFEXITED(currentStatus) || WEXITSTATUS(currentStatus)) foundSomeError = true; 
        if(!strcmp(target, buffer)) everythingIsOk = true;

    }
        
    if(foundSomeError) 
    {

        for (int i = 0; i < argc - 1; i++)
        {
            
            if (!finished[i])
            {

                kill(arrayOfPids[i], SIGTERM);
                waitpid(arrayOfPids[i], NULL, 0);

            }
            
        }

        exit(26);

    }

    if(everythingIsOk) exit(0);
    else exit(1);

}

