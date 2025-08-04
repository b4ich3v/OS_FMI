#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <err.h>

typedef struct pair
{
    int offset;
    int number;
} pair;

typedef struct packet
{
    pair pairs[4];
} packet;

int wrappaedOpen(const char* fileName, int mode)
{

    if(!fileName || mode < 0) err(1, "Invalid input");

    int fd = open(fileName, mode);
    if(fd < 0) err(1, "Open error");

    return fd;

}

int main(int argc, char* argv[]) 
{

	if(argc != 7) errx(1, "Wrong number of arguments");

    char buffer[16] = {0};

    int affix = wrappaedOpen(argv[1], O_RDONLY);
    if(read(affix, buffer, sizeof(buffer)) < 0) err(1, "Read error");

    int postfix = wrappaedOpen(argv[2], O_RDONLY);
    if(read(postfix, buffer, sizeof(buffer)) < 0) err(1, "Read error");

    int prefix = wrappaedOpen(argv[3], O_RDONLY);
    if(read(prefix, buffer, sizeof(buffer)) < 0) err(1, "Read error");

    int infix = wrappaedOpen(argv[4], O_RDONLY);
    if(read(infix, buffer, sizeof(buffer)) < 0) err(1, "Read error");

    int suffix = wrappaedOpen(argv[5], O_RDONLY);
    if(read(suffix, buffer, sizeof(buffer)) < 0) err(1, "Read error");

    int result = open(argv[6], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(result < 0) err(1, "Open error");

    struct stat fileStat;
    if(stat(argv[1], &fileStat) < 0) err(1, "Stat error");
    if(fileStat.st_size % sizeof(packet) != 0) err(1, "Invalid input");

    packet currentPacket;
    while(read(affix, &currentPacket, sizeof(packet)) > 0)
    {

        if(stat(argv[2], &fileStat) < 0) err(1, "Invalid input");
        if((long unsigned int)fileStat.st_size < currentPacket.pairs[0].offset * sizeof(uint32_t) + 
            currentPacket.pairs[0].number * sizeof(uint32_t)) err(1, "Invalid input");
        if(lseek(postfix, currentPacket.pairs[0].offset * sizeof(uint32_t), SEEK_SET) < 0) err(1, "Lseek error");
        uint32_t currentNumber1 = 0;

        for (int i = 0; i < currentPacket.pairs[0].number; i++)
        {
            
            if(read(postfix, &currentNumber1, sizeof(uint32_t)) < 0) err(1, "Read error");
            if(write(result, &currentNumber1, sizeof(uint32_t)) < 0) err(1, "Write error");

        }

        if(stat(argv[3], &fileStat) < 0) err(1, "Invalid input");
        if((long unsigned int)fileStat.st_size < currentPacket.pairs[1].offset * sizeof(uint8_t) + 
            currentPacket.pairs[1].number * sizeof(uint8_t)) err(1, "Invalid input");
        if(lseek(prefix, currentPacket.pairs[1].offset * sizeof(uint8_t), SEEK_SET) < 0) err(1, "Lseek error");
        uint8_t currentNumber2 = 0;

        for (int i = 0; i < currentPacket.pairs[1].number; i++)
        {
            
            if(read(prefix, &currentNumber2, sizeof(uint8_t)) < 0) err(1, "Read error");
            if(write(result, &currentNumber2, sizeof(uint8_t)) < 0) err(1, "Write error");

        }

        if(stat(argv[4], &fileStat) < 0) err(1, "Invalid input");
        if((long unsigned int)fileStat.st_size < currentPacket.pairs[2].offset * sizeof(uint16_t) + 
            currentPacket.pairs[2].number * sizeof(uint16_t)) err(1, "Invalid input");
        if(lseek(infix, currentPacket.pairs[2].offset * sizeof(uint16_t), SEEK_SET) < 0) err(1, "Lseek error");
        uint16_t currentNumber3 = 0;

        for (int i = 0; i < currentPacket.pairs[2].number; i++)
        {
            
            if(read(infix, &currentNumber3, sizeof(uint16_t)) < 0) err(1, "Read error");
            if(write(result, &currentNumber3, sizeof(uint16_t)) < 0) err(1, "Write error");

        }

        if(stat(argv[5], &fileStat) < 0) err(1, "Invalid input");
        if((long unsigned int)fileStat.st_size < currentPacket.pairs[3].offset * sizeof(uint64_t) + 
            currentPacket.pairs[3].number * sizeof(uint64_t)) err(1, "Invalid input");
        if(lseek(suffix, currentPacket.pairs[3].offset * sizeof(uint64_t), SEEK_SET) < 0) err(1, "Lseek error");
        uint64_t currentNumber4 = 0;

        for (int i = 0; i < currentPacket.pairs[3].number; i++)
        {
            
            if(read(suffix, &currentNumber4, sizeof(uint64_t)) < 0) err(1, "Read error");
            if(write(result, &currentNumber4, sizeof(uint64_t)) < 0) err(1, "Write error");

        }

    }

    close(affix);
    close(postfix);
    close(prefix);
    close(infix);
    close(suffix);
    close(result);

	return 0;

}
