#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>

int main(int argc, const char* argv[])
{

    if(argc != 2)
    {

        errx(1, "Error");

    }

    for (int i = 0; i < strtol((const char*) argv[1], (char**) NULL, 10); i++)
    {
        
        pid_t currentId = fork();

        if(currentId == 0)
        {

            
            write(1, "Child ", strlen("Child "));
            write(1, &i, sizeof(int));
            write(1, "\n", 1);
            
        }

    }

    exit(0);

}
