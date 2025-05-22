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

    if(argc != 2) errx(1, "Error");

    int sharedData[2];
    pipe(sharedData);

    int writtenBytes = write(sharedData[1], argv[1], strlen(argv[1]));
    if(writtenBytes < 0) { close(sharedData[0]); close(sharedData[1]); err(1, "Error"); }

    pid_t pid = fork();
    if(pid < 0) { close(sharedData[0]); close(sharedData[1]); err(1, "Error"); }

    if(pid == 0)
    {

        close(sharedData[1]);

        char buffer[100];
        int readBytes = read(sharedData[0], buffer, sizeof(buffer));
        if(readBytes < 0 ) { close(sharedData[0]); err(1, "Error"); }
        buffer[readBytes] = '\n';
        buffer[readBytes + 1] = '\0';

        write(1, buffer, strlen(buffer));
        close(sharedData[0]);
        exit(0);

    }

    int status = 0;
    wait(&status);
    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) { close(sharedData[0]); close(sharedData[1]); err(1, "Error"); }

    close(sharedData[0]);
    close(sharedData[1]);
    exit(0);

}
