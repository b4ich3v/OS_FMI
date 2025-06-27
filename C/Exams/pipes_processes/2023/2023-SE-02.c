#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    int countOfCommands = argc - 1;
    int* pids = malloc(sizeof(int) * countOfCommands);
    if(!pids) err(1, "Error");

    bool found = false;
    char buffer[1024];

    for (int i = 1; i <= countOfCommands; i++)
    {
        
        int sharedData[2];
        if(pipe(sharedData) < 0 ) err(1, "Error");

        int currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {

            close(sharedData[0]);
            if(dup2(sharedData[1], 1) < 0) err(1, "Error");
            close(sharedData[1]);

            execlp(argv[i], argv[i], (char*)NULL);
            err(1, "Error");

        }

        pids[i - 1] = currentPid;
        close(sharedData[1]);
        int readBytes = 0;
        if((readBytes = read(sharedData[0], buffer, sizeof("found it"))) < 0) err(1, "Error");
        close(sharedData[0]);

        buffer[readBytes] = '\0';

        if (strcmp(buffer, "found it") == 0) found = true;
        if(found)
        {

            for (int j = 0; j < countOfCommands; j++)
            {
                
                if(pids[j] > 0)
                {

                    kill(pids[j], SIGTERM);

                }

            }

            for (int j = 0; j < countOfCommands; j++)
            {
                
                if(waitpid(pids[j], NULL, 0) < 0 ) err(1, "Error");

            }

            free(pids);
            exit(0);

        }

    }
    
    for (int i = 0; i < countOfCommands; i++)
    {
        
        int status = 0;
        if(waitpid(pids[i], &status, 0) < 0 ) err(1, "Error");
        if(!WIFEXITED(status))
        {

            free(pids);
            exit(1);

        }
        

    }

    free(pids);
    exit(26);

}
