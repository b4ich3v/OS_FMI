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

    int currentStatus = 0;
    char readVariable = 'a';
    int counterForEndlines = 0;

    char buffer[10000];
    int bufferIndex = 0;

    while((currentStatus = read(fileFd, &readVariable, sizeof(char))) > 0)
    {

        if(counterForEndlines == 10) break;
        if(readVariable == '\n') 
        {

            counterForEndlines += 1;
            buffer[bufferIndex] = '\0';
            write(1, buffer, strlen(buffer) * sizeof(char));
            write(1, "\n", 1);
            bufferIndex = 0;

        }else
        {
        
            buffer[bufferIndex] = readVariable;
            bufferIndex += 1;   

        }

    }

    if (currentStatus < 0) perror("Error");

    close(fileFd);
    return 0;

}
