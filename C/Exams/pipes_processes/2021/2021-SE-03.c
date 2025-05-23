#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{

    if(argc != 3) errx(1, "Error");

    int fd1 = open(argv[1], O_RDONLY);
    if(fd1 < 0) errx(1, "Error");

    int fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(fd2 < 0) errx(1, "Error");

    int sizeOfFile = 0;
    uint8_t currentByte = 0;

    while((read(fd1, &currentByte, sizeof(uint8_t))) > 0)
    {

        sizeOfFile += 1;

    }

    if(lseek(fd1, 0, SEEK_SET) < 0) 
    {

        close(fd1);
        close(fd2);
        err(1, "Error");

    }

    int countOfNumbers = sizeOfFile / sizeof(uint16_t);
    
    if(countOfNumbers > 524288)
    {

        close(fd1);
        close(fd2);
        err(1, "Error");

    }

    uint16_t* array = malloc(countOfNumbers * sizeof(uint16_t));
    int index = 0;

    while((read(fd1, &currentByte, sizeof(uint8_t))) > 0)
    {

        array[index] = currentByte;
        index += 1;

    }

    dprintf(fd2, "#include <stdint.h>\n\n");
    dprintf(fd2, "int main(int argc, const char* argv[])\n\n");
    dprintf(fd2, "  const uint16_t arr[] = {\n");

    for (int i = 0; i < countOfNumbers; i++) 
    {

        dprintf(fd2, "0x%04x", array[i]);
        if (i < countOfNumbers - 1) dprintf(fd2, ",");

        if ((i + 1) % 8 == 0) dprintf(fd2, "\n");
        else dprintf(fd2, " ");

    }
    
    dprintf(fd2, "\n");
    dprintf(fd2, "  exit(0);\n");

    free(array);
    close(fd1);
    close(fd2);
    exit(0);

}
