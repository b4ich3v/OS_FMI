#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"  
#include "string.h"
#include "unistd.h" 
#include "fcntl.h" 

int main(int argc, const char* argv[])
{

    if(argc <= 1)
    {

        write(1, "Error\n", strlen("Error\n"));
        exit(1);

    }

    char buffer[1024];

    for (int i = 1; i < argc; i++)
    {
        
        int sourceFd = open(argv[i], O_RDONLY);
        int readBytes = 0;

        if((readBytes = read(sourceFd, buffer, 1024)) < 0 || sourceFd < 0)
        {

            close(sourceFd);
            write(1, "Error\n", strlen("Error\n"));
            exit(1);

        }

        write(1, buffer, readBytes);
        write(1, "\n\n", 2);

    }

    return 0;

}
