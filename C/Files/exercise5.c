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

    int sourceFd = open(argv[1], O_RDONLY);
    int destionationFd = open(argv[2], O_WRONLY | O_TRUNC);

    if(sourceFd < 0 || destionationFd < 0)
    {

        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    char buffer[1024];
    int bytesRead = 0;

    if((bytesRead = read(sourceFd, buffer, 1024)) < 0)
    {

        close(sourceFd);
        close(destionationFd);
        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    if(bytesRead < 0)
    {

        close(sourceFd);
        close(destionationFd);
        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    if(write(destionationFd, buffer, bytesRead) < 0)
    {

        close(sourceFd);
        close(destionationFd);
        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    close(sourceFd);
    close(destionationFd);

    return 0;

}
