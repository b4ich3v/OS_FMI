#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <err.h>

#define CHUNK_SIZE 16000000  
#define MAX_CHUNKS 32        

int helper(const void* first, const void* second) 
{

    uint32_t x = *(const uint32_t*)first;
    uint32_t y = *(const uint32_t*)second;

    return (x > y) - (x < y);

}

int main(int argc, char *argv[]) 
{

    if(argc != 2) errx(1, "Wrong number of arguments");
    char* inputFilename = argv[1];
    char* outputFilename = "sorted.bin";

    int readingFd = open(inputFilename, O_RDONLY);
    if(readingFd < 0) err(1, "Open error");

    uint32_t* buffer = malloc(CHUNK_SIZE * sizeof(uint32_t));
    if(buffer == NULL) err(1, "Null error");

    char temp_names[MAX_CHUNKS][32];
    int chunk_count = 0;
    ssize_t bytes_read;

    while((bytes_read = read(readingFd, buffer, CHUNK_SIZE * sizeof(uint32_t))) > 0) 
	{

        int count = bytes_read / sizeof(uint32_t);
        qsort(buffer, count, sizeof(uint32_t), helper);
        snprintf(temp_names[chunk_count], sizeof(temp_names[0]), "chunk_%d.tmp", chunk_count);

        int tempFd = open(temp_names[chunk_count], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if(tempFd < 0) err(1, "Open error");

        write(tempFd, buffer, count * sizeof(uint32_t));
        close(tempFd);

        chunk_count += 1;

    }

    close(readingFd);
    free(buffer);

    int writingFd = open(outputFilename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(writingFd < 0) err(1, "Open error");

    int chunkFds[MAX_CHUNKS];
    uint32_t values[MAX_CHUNKS];
    int active[MAX_CHUNKS] = {0};

    for(int i = 0; i < chunk_count; i++) 
	{

        chunkFds[i] = open(temp_names[i], O_RDONLY);
        if(chunkFds[i] < 0) err(1, "Invalid data");

        if(read(chunkFds[i], &values[i], sizeof(uint32_t)) == sizeof(uint32_t)) 
		{
        
			active[i] = 1;

        }

    }

    while(true) 
	{

        int minIndex = -1;

        for(int i = 0; i < chunk_count; i++) 
		{

            if (active[i]) 
			{

                if(minIndex == -1 || values[i] < values[minIndex]) 
				{

                    minIndex = i;

                }

            }

        }

        if (minIndex == -1) break;  
        write(writingFd, &values[minIndex], sizeof(uint32_t));

        if (read(chunkFds[minIndex], &values[minIndex], sizeof(uint32_t)) != sizeof(uint32_t)) 
		{

            active[minIndex] = 0;
            close(chunkFds[minIndex]);
            unlink(temp_names[minIndex]);  

        }

    }

    close(writingFd);
    return 0;

}
