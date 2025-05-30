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

    if(argc != 3) errx(1, "Error");

    char* ptr1;
    char* ptr2;

    int iterationNumber = strtol(argv[1], &ptr1, 10);
    int sleepNumber = strtol(argv[2], &ptr2, 10);

    int sharedData[2];

    for (int i = 0; i < iterationNumber; i++)
    {
        
        if(pipe(sharedData) < 0) err(1, "Error");

        pid_t currentPid = fork();

        if(currentPid < 0)
        {

            close(sharedData[0]);
            close(sharedData[1]);
            err(1, "Error");
            
        }

        if(currentPid == 0)
        {

            close(sharedData[1]);
            uint8_t dummyByte = 0;

            read(sharedData[0], &dummyByte, sizeof(uint8_t));
            write(1, "DONG\n", strlen("DONG\n"));

            close(sharedData[0]);
            exit(0);

        }

        close(sharedData[0]);
        uint8_t dummyByte = 47;

        write(1, "DING\n", strlen("DING\n"));
        write(sharedData[1], &dummyByte, sizeof(uint8_t));

        wait(NULL);
        sleep(sleepNumber);
        close(sharedData[1]);

    }

    exit(0);

}
