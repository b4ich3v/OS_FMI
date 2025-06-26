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

    if(argc != 4) errx(1, "Error");
    
    const char* program = argv[1];
    int countOfTests = strtol(argv[2], NULL, 10);

    int resultFd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(resultFd < 0) errx(1, "Error");

    int randomFd = open("/dev/urandom", O_RDONLY);
    if(randomFd < 0) errx(1, "Error");

    for (int i = 0; i < countOfTests; i++)
    {
        
        uint16_t randomNumber = 0;
        if(read(randomFd, &randomNumber, sizeof(uint16_t)) < 0) err(1, "Error");

        uint8_t bytes[65536];
        if(read(randomFd, bytes, randomNumber) < 0) err(1, "Error");

        int sharedData[2];
        if(pipe(sharedData) < 0) err(1, "Error");

        pid_t currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {
        
            close(sharedData[1]);
            int devNullFd = open("/dev/null", O_WRONLY);
            if(devNullFd < 0) err(1, "Error");

            if(dup2(sharedData[0], 0) < 0) err(1, "Error");
            close(sharedData[0]);

            if(dup2(devNullFd, 1) < 0 || dup2(devNullFd, 2) < 0) err(1, "Error");

            execlp(program, program, (char*)NULL);
            err(1, "Error");

        }

        close(sharedData[0]);
        if(write(sharedData[1], bytes, randomNumber) < 0) err(1, "Error");
        close(sharedData[1]);

        int status = 0;
        if(wait(&status) < 0) err(1, "Error");

        if(!WIFEXITED(status))
        {

            if(write(resultFd, bytes, randomNumber) < 0) err(1, "Error");

            close(randomFd);
            close(resultFd);
            exit(42);

        }

    }
    
    close(randomFd);
    close(resultFd);
    exit(0);

}
