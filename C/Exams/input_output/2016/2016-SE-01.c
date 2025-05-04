#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"  
#include "string.h"
#include "unistd.h"
#include <err.h> 

off_t getFileSize(int fd)
{

    off_t currentPosition = lseek(fd, 0, SEEK_CUR);

    if(currentPosition < 0 )
    {

        write(2, "Error");
        exit(1);
        close(fd);

    }

    off_t result = lseek(fd, 0, SEEK_END);

    if(result < 0 )
    {

        write(2, "Error");
        exit(1);
        close(fd);

    }

    if(lseek(fd, currentPosition, SEEK_SET) < 0 )
    {

        write(2, "Error");
        exit(1);
        close(fd);

    }

    return result;

}

void swap(char* c1, char* c2)
{

    char temp  = *c1;
    *c1 = *c2;
    *c2 = temp;

}

void bubbleSort(char* data, int size)
{

    for (int i = 0; i < size - 1; i++)
    {
        
        for (int j = 0; j < size - i - 1; j++)
        {
            
            if(data[j] > data[j + 1])
            {

                swap(&data[j], &data[j + 1]);

            }

        }

    }

}

int main(int argc, const char* argv[])
{

    if(argc != 2)
    {

        errx(2, "Error");
        exit(1);

    }

    int fd = open(argv[1], O_RDWR);

    if(fd < 0)
    {

        write(2, "Error");
        exit(1);

    }

    off_t fileSize = getFileSize(fd); 
    int index = 0;
    char fictiveByte = 'a';
    char* data = malloc(fileSize);
    ssize_t bytesRead = 0;

    while((bytesRead = read(fd, &fictiveByte, sizeof(char))) > 0)
    {

        data[index] = fictiveByte;
        index += 1;

    }

    if(bytesRead < 0)
    {

        close(fd);
        free(data);
        exit(1);

    }

    bubbleSort(data, (int)fileSize);
    off_t returnPosition = lseek(fd, 0, SEEK_SET);
    ssize_t bytesWritten = 0;

    if(returnPosition < 0)
    {

        close(fd);
        free(data);
        exit(1);

    }

    for (int i = 0; i < fileSize; i++)
    {
        
        if((bytesWritten = write(fd, &data[i], sizeof(char))) < 0)
        {

            close(fd);
            free(data);
            exit(1);

        }

    }
    
    if(bytesWritten < 0)
    {

        close(fd);
        free(data);
        exit(1);

    }

    close(fd);
    free(data);
    exit(0);

}
