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

    if(argc != 3) errx(1, "Error");

    int sourceFd = open(argv[1], O_RDONLY);
    if(sourceFd < 0) errx(1, "Error");

    int destFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(destFd < 0) errx(1, "Error");

    const uint8_t separator = 0x55;
    uint8_t currentByte = 0;

    while(read(sourceFd, &currentByte, sizeof(uint8_t)) > 0)
    {

        if((char)currentByte == separator)
        {

            uint8_t N = 0;
            uint8_t toleranceCounter = 0;
            uint8_t xorSum = 0;

            if(read(sourceFd, &N, sizeof(uint8_t)) < 0) err(1, "Error");

            while(read(sourceFd, &currentByte, sizeof(uint8_t)) > 0)
            {

                if((char)currentByte == separator) break;
                if(toleranceCounter > N - 3) err(1, "Error");

                xorSum ^= currentByte;
                toleranceCounter += 1;

            }

            uint8_t checkSumByte = 0;
            if(read(sourceFd, &checkSumByte, sizeof(uint8_t)) < 0) err(1, "Error");

            if(xorSum == checkSumByte)
            {

                uint8_t* data = malloc(N * sizeof(uint8_t));
                if(lseek(sourceFd, -N * sizeof(uint8_t), SEEK_CUR)) err(1, "Error");
                if(read(sourceFd, data, N * sizeof(uint8_t)) < 0) err(1, "Error");
                if(write(destFd, data, N * sizeof(uint8_t)) < 0) err(1, "Error");
                free(data);

            }

        }

    }

    close(sourceFd);
    close(destFd);
    exit(0);

}
