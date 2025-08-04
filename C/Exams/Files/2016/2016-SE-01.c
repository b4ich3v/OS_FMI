#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>

void insertionSort(uint8_t* arr, int size) 
{

    for (int i = 1; i < size; i++)
	{

		uint8_t key = arr[i];
        int j = i;

        while (j > 0 && arr[j - 1] > key) 
		{

            arr[j] = arr[j - 1];
            j--;

        }

        arr[j] = key;

    }

}

int main(int argc, char* argv[]) 
{

	if(argc != 2) errx(1, "Wrong number of arguments");

    int readingFd = open(argv[1], O_RDWR);
    if(readingFd < 0) err(1, "Open error");

    struct stat st;
    if(stat(argv[1], &st) < 0) err(1, "Stat error");

    int filesize = st.st_size;
    if(filesize == 0) err(1, "Invalid input");

    uint8_t* buffer = malloc(filesize);
    if (buffer == NULL) err(1, "Null error");

    ssize_t readBytes = read(readingFd, buffer, filesize);
    if (readBytes != filesize) err(1, "Invalid input");

    insertionSort(buffer, filesize);
    if(lseek(readingFd, 0, SEEK_SET) < 0) err(1, "Lseek error");

    ssize_t writtenBytes = write(readingFd, buffer, filesize);
    if(writtenBytes != filesize) err(1, "Invalid input");

    free(buffer);
    close(readingFd);
    return 0;

}
