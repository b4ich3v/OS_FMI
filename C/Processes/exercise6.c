#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, const char* argv[])
{

    if(argc != 2)
    {

        errx(1, "Error\n");

    }

    if(execlp(argv[1], argv[1], (char *)NULL) < 0)
    {

        err(1, "Error\n");

    }else
    {

        write(1, argv[1], strlen(argv[1]));

    }

    exit(0);

}
