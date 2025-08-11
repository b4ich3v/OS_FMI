#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>
#include <string.h>

typedef struct header
{
	uint64_t magic;
	uint32_t cfsb;
	uint32_t cfsu;
	uint32_t ofsb;
	uint32_t ofsu;
       	uint32_t unused;
	uint32_t cksum;
	uint8_t sectionkey[16];
	uint32_t offsets[4];	
} header;

typedef struct section 
{
    int64_t relative_offset; 
    uint64_t len;            
    uint8_t datakey[16];     
} section;

int main(int argc, char* argv[])
{
	
	if(argc != 3) errx(1, "Wrong number of arguments");

	int readingFd = open(argv[1], O_RDONLY);
	if(readingFd < 0) err(1, "Read error");

	int writingFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(writingFd < 0) err(1, "Write error");

	header h;
	if(read(readingFd, &h, sizeof(header)) < 0) err(1, "Read error");
	if(h.magic != 0x0000534f44614c47) err(1, "Invalid input data");
	
	for(int i = 0; i < 4; i++)
	{

		if(h.offsets[i] != 0)
		{

			section currentSection;
			if(lseek(readingFd, h.offsets[i] * 16, SEEK_SET) < 0) err(1, "Lseek error");
			if(read(readingFd, &currentSection, sizeof(section)) < 0) err(1, "Read error");
			
			for (int j = 0; j < sizeof(section); j++) 
			{

    			((uint8_t*)&currentSection)[j] ^= h.sectionkey[j % 16];

			}

			int64_t cryptdata_start = h.offsets[i] + currentSection.relative_offset;
    		uint64_t len = currentSection.len;
    		uint8_t buffer[16];

    		for (uint64_t j = 0; j < len; j++)
			{

        		off_t position = (cryptdata_start + j) * 16;
        		if (lseek(readingFd, position, SEEK_SET) < 0) err(1, "Lseek error");
        		if (read(readingFd, buffer, 16) < 0) err(1, "Read error");

        		for (int k = 0; k < 16; k++) 
				{

            		buffer[k] ^= currentSection.datakey[k];

        		}

        		if (write(writingFd, buffer, 16) < 0) err(1, "Write error");

    		}

		}

	}

	close(readingFd);
	close(writingFd);
	return 0;

}

