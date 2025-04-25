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

        write(1, "Error", strlen("Error"));
        exit(1);

    }

    int sourceFd = open(argv[1], O_RDONLY);
    int destinationFd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0644);

    if(sourceFd < 0 || destinationFd < 0)
    {

        perror("Error");
        exit(1);

    }

    int currentStatus;
    char buffer[1024];

    while((currentStatus = read(sourceFd, buffer, 1024 * sizeof(char))) > 0)
    {

        if (write(destinationFd, buffer, currentStatus) != currentStatus)
        {

            perror("Error");
            close(sourceFd);
            close(destinationFd);
            exit(1);

        }

    }

    if(!currentStatus) perror("Error");

    close(sourceFd);
    close(destinationFd);

    return 0;

}
