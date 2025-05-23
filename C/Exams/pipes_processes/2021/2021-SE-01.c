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

    int fd2 = open(argv[2], O_RDONLY);
    if(fd2 < 0) errx(1, "Error");

    uint8_t currentByte = 0;
    uint8_t dummyTrash = 0;
    uint16_t operatingMaskManipulation = 1;

    uint8_t helper1 = 0b01;
    uint8_t helper2 = 0b10;

    while((dummyTrash = read(fd1, &currentByte, sizeof(uint8_t))) > 0)
    {

        uint16_t reversedMask = 0;

        for (int i = 0; i < 8; i++)
        {
            
            if(currentByte & (operatingMaskManipulation < i))
            {

                reversedMask |= (helper1 << (2 * i));

            }
            else
            {
                
                reversedMask |= (helper2 << (2 * i));

            }

        }
     
        uint16_t result = 0;
        result |= (reversedMask << 4);
        result |= (reversedMask >> 4);
        write(fd2, &result, sizeof(uint16_t));

    }
    
    close(fd1);
    exit(0);

}
