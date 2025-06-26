#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdint.h>

typedef struct HelpingStruct
{

    uint32_t magic;
    uint32_t packetCount;
    uint64_t originalSize;

} HelpingStruct;

int main(int argc, char *argv[])
{

    if(argc != 3) errx(1, "Error");

    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);

    if(fd1 < 0 || fd2 < 0) errx(1, "Error");

    HelpingStruct helper;
    if(read(fd1, &helper, sizeof(HelpingStruct)) < 0) err(1, "Error");
    if(helper.magic != 0x21494D46) err(1, "Error");

    ssize_t writeBytes = 0;

    for (int i = 0; i < helper.packetCount; i++)
    {
        
        uint8_t currentPacketByte = 0;
        if(read(fd1, &currentPacketByte, sizeof(uint8_t)) < 0) err(1, "Error");

        bool isOne = (1 << 7) & currentPacketByte;
        int N = ((1 << 7) - 1) & currentPacketByte;
        uint8_t writingByte = 0;

        if(!isOne)
        {

            for (int i = 0; i < N + 1; i++)
            {
                
                if(read(fd1, &writingByte, sizeof(uint8_t)) < 0) err(1, "Error");
                if((writeBytes += write(fd2, &writingByte, sizeof(uint8_t))) < 0) err(1, "Error");

            }

        }
        else
        {

            if(read(fd1, &writingByte, sizeof(uint8_t)) < 0) err(1, "Error");

            for (int i = 0; i < N + 1; i++)
            {
                
                if((writeBytes += write(fd2, &writingByte, sizeof(uint8_t))) < 0) err(1, "Error");

            }
        
        }

    }
    
    if(writeBytes != helper.originalSize) err(1, "Error");

    close(fd1);
    close(fd2);
    exit(0);

}
