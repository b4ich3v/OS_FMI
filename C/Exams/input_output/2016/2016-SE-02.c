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

typedef struct Pair
{

    uint32_t number1;
    uint32_t number2;

} Pair;

int main(int argc, char *argv[])
{

    if(argc != 4) errx(1, "Error");

    int fd1 = open(argv[1], O_RDONLY);
    if(fd1 < 0) errx(1, "Error");

    int fd2 = open(argv[2], O_RDONLY);
    if(fd2 < 0) errx(1, "Error");

    int fd3 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(fd3 < 0) errx(1, "Error");

    Pair currentPair;
    struct stat st1, st2;

    if(fstat(fd1, &st1) < 0) err(1, "Error");
    if(fstat(fd2, &st2) < 0) err(1, "Error");   

    ssize_t sizeOfFile1 = st1.st_size;
    ssize_t sizeOfFile2 = st2.st_size;

    if(sizeOfFile1 % 8 != 0)
    {

        close(fd1);
        close(fd2);
        close(fd3);
        err(1, "Error");

    }

    while(read(fd1, &currentPair, sizeof(Pair)) > 0)
    {

        uint32_t startPosition = currentPair.number1;
        uint32_t offset = currentPair.number2;

        if(lseek(fd2, startPosition * sizeof(uint32_t), SEEK_SET) < 0)
        {

            close(fd1);
            close(fd2);
            close(fd3);
            err(1, "Error");

        }

        if((startPosition + offset) * sizeof(uint32_t) > sizeOfFile2)
        {

            close(fd1);
            close(fd2);
            close(fd3);
            err(1, "Error");

        }
        
    }

    if(lseek(fd1, 0, SEEK_SET) < 0) err(1, "Error");

    while(read(fd1, &currentPair, sizeof(Pair)) > 0)
    {

        uint32_t startPosition = currentPair.number1;
        uint32_t offset = currentPair.number2;

        if(lseek(fd2, startPosition * sizeof(uint32_t), SEEK_SET) < 0)
        {

            close(fd1);
            close(fd2);
            close(fd3);
            err(1, "Error");

        }

        if((startPosition + offset) * sizeof(uint32_t) > sizeOfFile2)
        {

            close(fd1);
            close(fd2);
            close(fd3);
            err(1, "Error");

        }

        for (int i = 0; i < offset; i++)
        {
            
            uint32_t readByte = 0;

            if(read(fd2, &readByte, sizeof(uint32_t)) < 0)
            {

                close(fd1);
                close(fd2);
                close(fd3);
                err(1, "Error");

            }

            if(write(fd3, &readByte, sizeof(uint32_t)) < 0)
            {

                close(fd1);
                close(fd2);
                close(fd3);
                err(1, "Error");

            }

        }
        
    }

    close(fd1);
    close(fd2);
    close(fd3);
    exit(0);

}
