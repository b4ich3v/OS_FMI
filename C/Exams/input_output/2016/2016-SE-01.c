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

    if(argc != 2) errx(1, "Error");

    struct stat st;
    ssize_t sizeOfFile = st.st_size;
    
    if(sizeOfFile < 0) errx(1, "Error");

    int readingFd = open(argv[1], O_RDONLY);
    if(readingFd < 0) errx(1, "Error");

    uint8_t* array = malloc(sizeof(uint8_t) * sizeOfFile);
    uint8_t currentByte = 0;
    int8_t fictiveData = 0;
    ssize_t currentIndex = 0;

    while((fictiveData = read(readingFd, &currentByte, sizeof(uint8_t))) > 0)
    {

        if(fictiveData < 0)
        {

            close(readingFd);
            free(array);
            err(1, "Error");

        }

        array[currentIndex] = currentByte;
        currentIndex += 1;

    }

    close(readingFd);

    int writingFd = open(argv[1], O_WRONLY | O_TRUNC | 0644);
    if(writingFd < 0) 
    {

        free(array);
        err(1, "Error");

    }

    for (int i = 0; i < sizeOfFile - 1; i++)
    {
        
        for (int j = 0; j < sizeOfFile - j - 1; j++)
        {
            
            if(array[j] < array[j + 1])
            {

                uint8_t temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

            }

        }
        
    }
    
    if(write(writingFd, array, sizeof(uint8_t) * sizeOfFile) < 0)
    {

        free(array);
        err(1, "Error");

    }

    close(writingFd);
    free(array);
    exit(0);

}
