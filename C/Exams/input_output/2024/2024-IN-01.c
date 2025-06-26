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

typedef struct Header
{

    uint16_t magic;
    uint16_t ver;
    uint16_t cp;
    uint16_t co;
    
} Header;

typedef struct Preamble
{

    uint16_t v1;
    uint16_t v2;
    uint32_t v3;

} Preamble;

typedef struct Object 
{

    uint32_t ctime;    
    uint16_t opt;       
    uint16_t parent_id; 
    uint32_t size;      
    uint32_t ssize;  

} Object;

int main(int argc, char *argv[])
{

    if(argc != 2) errx(1, "Error");

    int fd = open(argv[1], O_RDONLY);
    if(fd < 0) errx(1, "Error");

    Header header;

    if(read(fd, &header, sizeof(Header)) < 0) err(1, "Error");
    if(lseek(fd, header.cp * sizeof(Preamble), SEEK_CUR) < 0) err(1, "Error");

    Object* objects = malloc(header.co * sizeof(Object));
    if(read(fd, objects, header.co * sizeof(Object)) < 0) err(1, "Error");

    uint64_t sum_ssize = 0;
    uint64_t sum_size = 0;

    for (int i = 0; i < header.co; i++)
    {

        uint16_t type = (objects[i].opt >> 14) & 0x3;
        if (type != 0b10) continue;  

        uint16_t pid = objects[i].parent_id;
        if (pid == 0 || pid >= header.co) continue;

        uint32_t time1 = objects[i].ctime;
        uint32_t time2 = objects[pid].ctime;
        uint32_t delta = (time1 > time2) ? (time1 - time2) : (time2 - time1);

        if (delta >= 85800 && delta <= 87000) 
        {

            sum_ssize += objects[i].ssize;
            sum_size  += objects[i].size;

        }

    }
    
    if (sum_size == 0) printf("0.000000\n");
    else printf("%.6f\n", (double)sum_ssize / sum_size);

    free(objects);
    close(fd);
    exit(0);

}
