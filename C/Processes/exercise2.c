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
        exit(1);

    }

    if(execlp("ls", "ls", argv[1], (char*) NULL) < 0)
    {

        err(1, "Error");
        exit(1);

    };

    exit(0);

}
