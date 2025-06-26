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
    
    int fd = open(argv[2], O_RDONLY);
    if(fd < 0) errx(1, "Error");

    int countOfWords = 0;
    uint8_t fictiveByte;
    while(read(fd, &fictiveByte, sizeof(uint8_t)) > 0) 
    {

        if(fictiveByte == 0x00) 
        {

            countOfWords += 1;

        }

    }

    const char* target = argv[1];
    const char sep1 = 0x00;
    const char sep2 = '\n';

    ssize_t left = 0;
    ssize_t right = countOfWords - 1;
    bool found = false;
    char wordBuffer[64];

    while(left <= right)
    {

        ssize_t middle = left + (right - left) / 2;
        ssize_t counter = 0;
        char currentByte = 0;
        if(lseek(fd, 0, SEEK_SET) < 0) err(1, "Error");

        while(counter < middle)
        {

            if(read(fd, &currentByte, sizeof(char)) < 0) err(1, "Error");
            if((char)currentByte == sep1) counter += 1;

        }

        ssize_t currentIndex = 0;

        while(read(fd, &currentByte, sizeof(char)) > 0)
        {

            if((char)currentByte == sep2) break;
            if (currentIndex >= sizeof(wordBuffer) - 1) err(1, "Error");

            wordBuffer[currentIndex] = (char)currentByte;
            currentIndex += 1;

        }

        wordBuffer[currentIndex] = '\0';
        int pred = strcmp(wordBuffer, target);

        if(pred < 0) left = middle + 1;
        else if(pred > 0) right = middle - 1;
        else
        {

            while(read(fd, &currentByte, sizeof(uint8_t)) > 0)
            {

                if((char)currentByte == sep1) break;
                write(1, &currentByte, sizeof(uint8_t));

            }

            write(1, "\n", strlen("\n"));
            found = true;
            break;

        }

    }

    if(found) write(1, "Found\n", strlen("Found\n"));
    else write(1, "Not found\n", strlen("Not found\n"));

    close(fd);
    exit(0);

}
