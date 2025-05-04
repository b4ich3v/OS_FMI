#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "fcntl.h"  
#include "string.h"
#include "unistd.h"
#include <err.h> 

typedef struct
{

    uint32_t number1;
    uint32_t number2;

} Pair;

off_t getFileSize(int fd)
{

    off_t currentPos = lseek(fd, 0, SEEK_CUR);
    if(currentPos < 0) { write(2, "Error\n", 6); exit(1); }
    
    off_t result = lseek(fd, 0, SEEK_END);
    if(result < 0) { write(2, "Error\n", 6); exit(1); }

    if(lseek(fd, currentPos, SEEK_SET) < 0) { write(2, "Error\n", 6); exit(1); }

    return result;

}

void freeMethodErr(int fd1, int fd2, int fd3)
{

    write(2, "Error\n", 6);

    close(fd1);
    close(fd2);
    close(fd3);

    exit(1);

}

int main(int argc, const char* argv[])
{

    if(argc != 4)
    {

        errx(2, "Error\n");
        exit(1);

    }

    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_RDONLY);
    int fd3 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);

    off_t fileSize = getFileSize(fd1);
    int countOfPairs = fileSize / 8;
    Pair pair;

    if(fileSize % 8 != 0) freeMethodErr(fd1, fd2, fd3);

    for (int i = 0; i < countOfPairs; i++)
    {

        if(read(fd1, &pair, sizeof(Pair)) < 0) freeMethodErr(fd1, fd2, fd3);
        
        off_t wantedPos = lseek(fd2, (off_t)pair.number1 * sizeof(uint32_t), SEEK_SET);
        if(wantedPos < 0) freeMethodErr(fd1, fd2, fd3);

        uint32_t value = 0;

        for (int j = 0; j < pair.number2; j++)
        {
            
            if(read(fd2, &value, sizeof(uint32_t)) < 0) freeMethodErr(fd1, fd2, fd3);
            if(write(fd3, &value, sizeof(uint32_t)) < 0 ) freeMethodErr(fd1, fd2, fd3);

        }

        if(lseek(fd2, 0, SEEK_SET)) freeMethodErr(fd1, fd2, fd3);

    }
    
    close(fd1);
    close(fd2);
    close(fd3);
    exit(0);

}
