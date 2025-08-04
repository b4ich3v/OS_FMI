#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <err.h>

typedef struct version1
{ 
	uint16_t offset;
	uint8_t originalByte;
	uint8_t newByte;
} version1;

typedef struct version2
{ 
	uint32_t offset;
	uint16_t originalWord;
	uint16_t newWord;
} version2;

typedef struct header
{
	uint32_t magic;
	uint8_t headerVersion;
	uint8_t dataVersion;
	uint16_t count;
    uint32_t reserved1;
	uint32_t reserved2;	
} header;

int main(int argc, char* argv[]) 
{

	
	if(argc != 4) errx(1, "Wrong number of arguments");

	int patchFd = open(argv[1], O_RDONLY);
	if(patchFd < 0) err(1, "Open error");

	int writingFd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Open error");

	header h;
	if(read(patchFd, &h, sizeof(header)) < 0) err(1, "Read error");
	if(h.magic != 0xEFBEADDE || h.headerVersion != 0x01) err(1, "Invalid input");

	int size = h.count;

	if(h.dataVersion == 0x00)
	{

        int readingFd = open(argv[2], O_RDONLY);
	    if(readingFd < 0) err(1, "Open error");

        struct stat fileStat;
        if(stat(argv[2], &fileStat) < 0) err(1, "Stat error");
        if(fileStat.st_size % sizeof(uint8_t) != 0) err(1, "Invalid input");
    
		version1* elements = malloc(sizeof(version1) * size);
        if(elements == NULL) err(1, "Null error");
		int index = 0;

		while(read(patchFd, &elements[index], sizeof(version1)) > 0) index += 1;

        for (int i = 0; i < size; i++)
        {
            
            if(fileStat.st_size < elements[i].offset) err(1, "Invalid input");

        }

        uint8_t currentNumber;
        index = 0;

		while(read(readingFd, &currentNumber, sizeof(uint8_t)) > 0)
        {

            bool hasPatch = false;

            for (int i = 0; i < size; i++)
            {
            
                if((int)elements[i].offset == index && elements[i].originalByte == currentNumber)
                {

                    if(write(writingFd, &elements[i].newByte, sizeof(uint8_t)) < 0) err(1, "Write error");
                    hasPatch = true;
                    break;

                }

            }
            
            if(!hasPatch) 
            {

                if(write(writingFd, &currentNumber, sizeof(uint8_t)) < 0) err(1, "Write error");

            }

            index += 1;

        }

		free(elements);
        close(readingFd);

	}
	else if(h.dataVersion == 0x01)
	{

        int readingFd = open(argv[2], O_RDONLY);
	    if(readingFd < 0) err(1, "Open error");

        struct stat fileStat;
        if(stat(argv[2], &fileStat) < 0) err(1, "Stat error");
        if(fileStat.st_size % sizeof(uint16_t) != 0) err(1, "Invalid input");
    
		version2* elements = malloc(sizeof(version2) * size);
        if(elements == NULL) err(1, "Null error");
		int index = 0;

		while(read(patchFd, &elements[index], sizeof(version2)) > 0) index += 1;

        for (int i = 0; i < size; i++)
        {
            
            if(fileStat.st_size < elements[i].offset) err(1, "Invalid input");

        }

        uint16_t currentNumber;
        index = 0;

		while(read(readingFd, &currentNumber, sizeof(uint16_t)) > 0)
        {

            bool hasPatch = false;

            for (int i = 0; i < size; i++)
            {
            
                if((int)elements[i].offset == index && elements[i].originalWord == currentNumber)
                {

                    if(write(writingFd, &elements[i].newWord, sizeof(uint16_t)) < 0) err(1, "Write error");
                    hasPatch = true;
                    break;

                }

            }
            
            if(!hasPatch) 
            {

                if(write(writingFd, &currentNumber, sizeof(uint16_t)) < 0) err(1, "Write error");

            }

            index += 1;

        }

		free(elements);
        close(readingFd);

	}
	else
	{
		
		err(1, "Invalid input");

	}
    
	close(patchFd);
	return 0;

}
