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

    int readingFd = open(argv[1], O_RDONLY);
    if(readingFd < 0) errx(1, "Error");

    int writingFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(writingFd < 0) errx(1, "Error");

    uint8_t currentByte = 0;
    uint8_t target = 0x7D;
    uint8_t xorOperator = 0x20;

    while(read(readingFd, &currentByte, sizeof(uint8_t)) > 0)
    {

        if(currentByte == target)
        {

            uint8_t nextByte = 0;

            if(read(readingFd, &nextByte, sizeof(uint8_t)) < 0)
            {

                close(readingFd);
                close(writingFd);
                err(1, "Error");

            }
            nextByte ^= xorOperator;

            write(writingFd, &nextByte, sizeof(uint8_t));
            break;

        }
        else
        {

            write(writingFd, &currentByte, sizeof(uint8_t));

        }

    }

    close(readingFd);
    close(writingFd);
    exit(0);

}
