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

    if(argc != 3) errx(1, "Error");

    int fds[2];
    pipe(fds);

    pid_t pid1 = fork();
    if(pid1 < 0) err(1, "Error");

    if(pid1 == 0)
    {

        close(fds[0]);                       
        if (dup2(fds[1], STDOUT_FILENO) < 0) err(1, "dup2");
            
        close(fds[1]);                       
        execlp(argv[1], argv[1], (char*) NULL);      
        err(1, "Error");        

    }

    int status = 0;
    waitpid(pid1, &status, 0);
    if(!WIFEXITED(status)) err(1, "Error");

    char buffer[100];
    int readBytes = 0;
    if((readBytes = read(fds[0], buffer, sizeof(buffer))) < 0) err(1, "Error");
    buffer[readBytes] ='\0'; 
    if(!strcmp(buffer, "Broski")) err(1, "Error");

    pid_t pid2 = fork();
    if(pid2 < 0) err(1, "Error");

    if(pid2 == 0)
    {

        close(fds[0]);                       
        if (dup2(fds[1], STDOUT_FILENO) < 0) err(1, "dup2");
            
        close(fds[1]);                       
        execlp(argv[2], argv[2], (char*) NULL);     
        err(1, "Error");

    }

    waitpid(pid2, &status, 0);
    if(!WIFEXITED(status)) err(1, "Error");

    if((readBytes = read(fds[0], buffer, sizeof(buffer))) < 0) err(1, "Error");
    buffer[readBytes] ='\0';
    if(!strcmp(buffer, "Broski")) err(1, "Error");

    close(fds[0]);
    close(fds[1]);
    exit(42);

}
