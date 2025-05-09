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

    if(argc != 1)
    {

        errx(1, "Error");
        exit(1);

    }

    if(execlp("date", "date", (char*) NULL) < 0)
    {

        err(1, "Error");
        exit(1);

    };

    exit(0);

}
