#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"  
#include "string.h"
#include "unistd.h" 
#include "fcntl.h" 

int main(int argc, const char* argv[])
{

    
    if(argc != 2)
    {

        write(1, "Error", strlen("Error"));
        exit(1);

    }

    int fileFd = open(argv[1], O_RDONLY);

    if(fileFd < 0)
    {

        perror("Error");
        exit(1);

    }

    int counter = 0;
    char target = '\n';
    char currentSymbol = 'a';
    int currentStatus = 0;

    while((currentStatus = read(fileFd, &currentSymbol, sizeof(char))) > 0)
    {

        if(currentSymbol == target) counter += 1;

    }

    if(currentStatus < 0)
    {

        perror("Error");
        exit(1);

    }

    printf("The result is %d \n", counter + 1);    
    close(fileFd);
    return 0;

}
