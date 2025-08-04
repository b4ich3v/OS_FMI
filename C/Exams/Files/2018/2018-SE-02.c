#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define CHUNK_SIZE 1000000 

void insertionSort(uint32_t* arr, int size) 
{

    for(int i = 1; i < size; i++)
	{

		uint32_t key = arr[i];
        int j = i;

        while(j > 0 && arr[j - 1] > key) 
		{

            arr[j] = arr[j - 1];
            j--;

        }

        arr[j] = key;

    }

}

int main(int argc, char *argv[]) 
{

    if(argc != 3) errx(1, "Wrong number of arguments");

    int readingFd = open(argv[1], O_RDONLY);
    if(readingFd < 0) err(1, "Open error");

    int writingFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(writingFd < 0) err(1, "Open error");

    uint32_t* buffer = malloc(CHUNK_SIZE * sizeof(uint32_t));
    if(buffer == NULL) err(1, "Null error");

    struct stat st;
    if(stat(argv[1], &st) < 0) err(1, "Stat error");

    int totalBytes = st.st_size;
    if(totalBytes % sizeof(uint32_t) != 0) err(1, "Invalid input");

    int totalNumbers = totalBytes / sizeof(uint32_t);
    int numbersRead = 0;

    while(numbersRead < totalNumbers) 
	{

        int toRead = CHUNK_SIZE;

        if(totalNumbers - numbersRead < CHUNK_SIZE)
            toRead = totalNumbers - numbersRead;

        ssize_t bytes = read(readingFd, buffer, toRead * sizeof(uint32_t));

        if(bytes < 0) err(1, "Invalid input");

        int nums = bytes / sizeof(uint32_t);
        insertionSort(buffer, nums);

        ssize_t written = write(writingFd, buffer, nums * sizeof(uint32_t));

        if(written < 0) err(1, "Invalid input");
        numbersRead += nums;

    }

    free(buffer);
    close(readingFd);
    close(writingFd);

    return 0;

}
