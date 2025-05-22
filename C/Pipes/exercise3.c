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
    
    pid_t pid = fork();
    if(pid < 0) { close(sharedData[0]); close(sharedData[1]); err(1, "Error"); }

    if(pid == 0)
    {

        close(sharedData[1]);
        dup2(sharedData[0], STDIN_FILENO);
        close(sharedData[0]);

        execlp("wc", "wc", "-c", (char*)NULL);
        err(1, "Error");

    }

    close(sharedData[0]);
    if (write(sharedData[1], argv[1], strlen(argv[1])) < 0) { close(sharedData[1]); err(1, "Error"); };
    close(sharedData[1]);

    int status = 0;
    wait(&status);
    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) err(1, "Error");

    exit(0);

}
