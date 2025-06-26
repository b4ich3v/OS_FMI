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

typedef struct Node
{

    uint64_t next;
    uint8_t data[512];

} Node;

int main(int argc, char *argv[])
{

    if(argc != 2) errx(1, "Error");

    int fd = open(argv[1], O_RDWR);
    if(fd < 0) errx(1, "Error");

    char template[] = "tempFileXXXXXX";
    int tempFd = mkstemp(template);

    struct stat st;
    if(fstat(fd, &st) < 0) err(1, "Error");
    ssize_t sizeOfFile = st.st_size;

    Node currentNode;
    
    if(read(fd, &currentNode, sizeof(Node)) < 0) err(1, "Error");
    if(write(tempFd, &currentNode.next, sizeof(uint64_t)) < 0) err(1, "Error");
    uint64_t maxNumber = 0;

    while(true)
    {

        if(currentNode.next * sizeof(Node) > sizeOfFile) break;
        if(currentNode.next > maxNumber) maxNumber = currentNode.next;

        if(lseek(fd, currentNode.next * sizeof(Node), SEEK_SET) < 0) err(1, "Error");
        if(write(tempFd, &currentNode.next, sizeof(uint64_t)) < 0) err(1, "Error");
        if(read(fd, &currentNode, sizeof(Node)) < 0) err(1, "Error");       
        
    }

    char replacementBuffer[512] = {0};

    for (int i = 1; i < maxNumber; i++)
    {
        
        bool found = false;
        uint64_t current = 0;

        while(read(tempFd, &current, sizeof(uint64_t)) > 0)
        {

            if(current == i)
            {

                found = true;
                break;

            }

        }

        if(!found)
        {

            if(lseek(fd, i * sizeof(Node), SEEK_SET) < 0) err(1, "Error");
            if(write(fd, replacementBuffer, sizeof(replacementBuffer)) < 0) err(1, "Error");

        }

    }

    unlink(template);
    close(tempFd);
    close(fd);
    exit(0);

}
