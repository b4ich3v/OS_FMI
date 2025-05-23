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
    
    uint8_t currentByte = 0;

    while(read(fd1, &currentByte, sizeof(uint8_t)) > 0)
    {

        uint16_t currentTwoBytesMask = 0;

        for (int i = 0; i < 8; i++)
        {
            
            if(currentByte & (1 << (7 - i)))
            {

                currentTwoBytesMask |= (0b10 << (2 * i));

            }
            else
            {
            
                currentTwoBytesMask |= (0b01 << (2 * i));

            }

        }

        write(fd2, &currentTwoBytesMask, sizeof(uint16_t));

    }

    close(fd1);
    close(fd2);
    exit(0);

}
