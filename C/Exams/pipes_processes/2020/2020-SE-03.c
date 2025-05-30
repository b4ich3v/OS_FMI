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

typedef struct Tuple
{

    char fileName[8];
    uint32_t offset;
    uint32_t countOfElements;


} Tuple;

int main(int argc, char *argv[])
{

    if(argc != 2) errx(1, "Error");

    int startingReadingFd = open(argv[1], O_RDONLY);
    if(startingReadingFd < 0) errx(1, "Error");

    int countOfTuples = 0;
    Tuple currentParentTuple;

    while(read(startingReadingFd, &currentParentTuple, sizeof(Tuple)) > 0)
    {

        countOfTuples += 1;

    }
    
    if(countOfTuples > 8) err(1, "Error");
    if(lseek(startingReadingFd, 0, SEEK_SET) < 0 ) err(1, "Error");
    uint32_t result = 0;
    int sharedData[2];

    while(read(startingReadingFd, &currentParentTuple, sizeof(Tuple)) > 0)
    {

        if(pipe(sharedData) < 0) err(1, "Error");

        pid_t currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {

            Tuple currentChildTuple;
            read(sharedData[0], &currentChildTuple, sizeof(Tuple));
            close(sharedData[0]);

            for (int i = 0; i < 8; i++)
            {
            
                if(currentChildTuple.fileName[i] == 0x00)
                {

                    currentChildTuple.fileName[i] = '\0';
                    break;

                }

            }
        
            int currentReadingFd = open(currentChildTuple.fileName, O_RDONLY);

            if(currentReadingFd < 0) 
            {

                close(startingReadingFd);
                err(1, "Error");

            }

            uint8_t currentByte = 0;
            uint32_t currentMask = 0;

            if(lseek(currentReadingFd, currentChildTuple.offset, SEEK_SET) < 0) err(1, "Error");
        
            for (int i = 0; i < currentChildTuple.countOfElements; i++)
            {
            
                read(currentReadingFd, &currentByte, sizeof(uint8_t));
                currentMask ^= currentByte;

            }

            write(sharedData[1], &currentMask, sizeof(uint32_t));
            close(sharedData[1]);
            exit(0);

        }

        uint32_t currenChildResult = 0;
        write(sharedData[1], &currentParentTuple, sizeof(Tuple));
        read(sharedData[0], &currenChildResult, sizeof(uint32_t));
        result ^= currenChildResult;

        close(sharedData[0]);
        close(sharedData[1]);

    }

    for (int i = 0; i < countOfTuples; i++)
    {
        
        int currentStatus = 0;
        wait(&currentStatus);

        if(!WIFEXITED(currentStatus) || WEXITSTATUS(currentStatus) != 0) err(1, "Error");

    }
    
    write(1, &result, sizeof(uint32_t));
    close(startingReadingFd);
    exit(0);

}

