#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"  
#include "string.h"
#include "unistd.h" 
#include "fcntl.h" 

int main(int argc, const char* argv[])
{

    if(argc != 3)
    {

        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    int file1Fd = open(argv[1], O_RDWR);
    int file2Fd = open(argv[2], O_RDWR);

    if(file1Fd < 0 || file2Fd < 0)
    {

        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    char bufferForFile1[1024];
    char bufferForFile2[1024];
    int readBytes1 = 0;
    int readBytes2 = 0;
    char currentSymbol = 'a';

    if(!((readBytes1 = read(file1Fd, bufferForFile1, sizeof(bufferForFile1))) > 0)) 
    {

        close(file1Fd);
        close(file2Fd);
        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    if (readBytes1 < 0) write(1, "Error\n", strlen("Error\n"));
    ftruncate(file1Fd, 0);
    lseek(file1Fd, 0, SEEK_SET);

    if(!((readBytes2 = read(file2Fd, bufferForFile2, sizeof(bufferForFile2))) > 0)) 
    {

            close(file1Fd);
            close(file2Fd);
            write(1, "Error\n", strlen("Error\n"));
            exit(1);

    }

    if (readBytes2 < 0) write(1, "Error\n", strlen("Error\n"));
    ftruncate(file2Fd, 0); 
    lseek(file2Fd, 0, SEEK_SET);
   
    if(write(file2Fd, bufferForFile1, readBytes1) < 0)
    {

        close(file1Fd);
        close(file2Fd);
        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    if(write(file1Fd, bufferForFile2, readBytes2) < 0)
    {

        close(file1Fd);
        close(file2Fd);
        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    close(file1Fd);
    close(file2Fd);

    return 0;

}
