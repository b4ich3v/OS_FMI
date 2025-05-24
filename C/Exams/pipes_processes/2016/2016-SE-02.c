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

    const char* endString = "exit";
    char bufferForCurrentCommand[1024];
    int currentBytes = 0;

    while(true)
    {

        write(1, ">", sizeof(">"));
        currentBytes = read(0, bufferForCurrentCommand, sizeof(bufferForCurrentCommand));

        if (currentBytes < 0) err(1, "Error");
        if (currentBytes == 0) continue;
        if (bufferForCurrentCommand[currentBytes - 1] == '\n') bufferForCurrentCommand[currentBytes - 1] = '\0';
        if(!strcmp(bufferForCurrentCommand, endString)) break;

        pid_t currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {

            execlp(bufferForCurrentCommand, bufferForCurrentCommand, (char*) NULL);
            err(1, "Error");

        }

        int status = 0;
        wait(&status);

        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Error");
    
    }

    exit(0);

}
