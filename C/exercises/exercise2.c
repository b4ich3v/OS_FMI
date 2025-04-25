#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"  
#include "string.h"
#include "unistd.h" 
#include "fcntl.h" 

void freeBuffer(char* ptr)
{

    ptr[0] = '\0';

}

int main(int argc, const char* argv[])
{

    if(argc != 2)
    {

        write(1, "Error", strlen("Error"));
        exit(1);

    }

    int fileFd = open(argv[1], O_RDONLY);
    int currentStatus = 0;
    char readVariable = 'a';
    int counterForEndlines = 0;

    char buffer[1024];
    int bufferIndex = 0;

    while((currentStatus = read(fileFd, &readVariable, sizeof(char)) > 0))
    {

        if(counterForEndlines == 10) break;
        if(readVariable == '\n') 
        {

            counterForEndlines += 1;
            buffer[bufferIndex] = '\0';
            write(1, buffer, strlen(buffer) * sizeof(char));
            freeBuffer(buffer);
            bufferIndex = 0;

        }else
        {
        
            buffer[bufferIndex] = readVariable;
            bufferIndex += 1;   

        }

    }

    return 0;

}
