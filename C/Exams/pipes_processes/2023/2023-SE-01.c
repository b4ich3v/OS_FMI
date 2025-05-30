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

    if(argc != 2) errx(1, "Error"); 

    char* commands[][20] = 
    {

        {"find", argv[1], "-type", "f", "-exec", "md5sum", "{}", ";", NULL},
        {"grep", "-v", ".hash", NULL},
        {"tr", "-s", " ", NULL},
        {NULL}

    };

    int countOfCommands = 3;
    int prevFd = -1;
    int sharedData[2];

    int writingFd = open("result.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(writingFd < 0) err(1, "Error");

    for (int i = 0; i < countOfCommands; i++)
    {
        
        if(i != countOfCommands - 1) 
        {

            if(pipe(sharedData) < 0 ) err(1, "Error");

        }

        pid_t currentPid = fork();
        if(currentPid < 0 ) err(1, "Error");

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
            else
            {
            
                dup2(writingFd, STDOUT_FILENO);
                close(writingFd);

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
        
        wait(NULL);

    }
    
    int readingFd = open("result.txt", O_RDONLY);
    if (readingFd < 0) err(1, "Error");

    char currentLine[2048];
    int lineIndex = 0;
    char currentByte = 'a';

    while (read(readingFd, &currentByte, 1) == 1)
     {

        if (currentByte == '\n')
        {

            currentLine[lineIndex] = '\0';  

            pid_t pid = fork();
            if (pid < 0) err(1, "Error");

            if (pid == 0)
            {
            
                char hash[128]; 
                char path[1024];
                int i = 0;

                while (currentLine[i] != ' ' && currentLine[i] != '\0' && i < sizeof(hash) - 1) 
                {

                    hash[i] = currentLine[i];
                    i += 1;

                }

                hash[i] = '\0';
                while (currentLine[i] == ' ') i += 1;

                int j = 0;

                while (currentLine[i] != '\0' && j < sizeof(path) - 1)
                {

                    path[j++] = currentLine[i++];

                }

                path[j] = '\0';
                char hashFile[1100];
                snprintf(hashFile, sizeof(hashFile), "%s.hash", path);

                int outFd = open(hashFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (outFd < 0) err(3, "Cannot create hash file");

                if (write(outFd, hash, strlen(hash)) < 0 ||
                    write(outFd, "\n", 1) < 0) 
                {

                    err(1, "Error");

                }

                close(outFd);
                exit(0);

            }

            lineIndex = 0;
        } 
        else 
        {

            if (lineIndex < 2048 - 1) 
            {

                currentLine[lineIndex++] = currentByte;

            }

        }

    }

    close(readingFd);
    while (wait(NULL) > 0);

    exit(0);

}
