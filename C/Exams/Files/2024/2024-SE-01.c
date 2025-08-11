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

typedef struct node
{
	uint64_t next;
	uint8_t user_data[508];
} node;

int main(int argc, char* argv[])
{
	
	if(argc != 2) errx(1, "Wrong number of arguments");
	
	int fd = open(argv[1], O_RDWR);
	if(fd < 0) err(1, "Open error");

	struct stat fileStat;
	if(stat(argv[1], &fileStat) < 0) err(1, "Stat error");
	
	if(fileStat.st_size % sizeof(node) != 0) err(1, "Invalid data input");
	int countOfNodes = fileStat.st_size / sizeof(node);

	bool* visited = malloc(countOfNodes * sizeof(bool));
	if(visited == NULL) err(1, "Null error");
	for (int i = 0; i < countOfNodes; i++) visited[i] = false;
	
	node currentNode;
	int currentIndex = 0;

	while (true) 
	{

    	if (currentIndex >= countOfNodes) errx(1, "Invalid next index");
    	if (visited[currentIndex]) break; 

    	visited[currentIndex] = true;

    	if (lseek(fd, currentIndex * sizeof(node), SEEK_SET) < 0) err(1, "Lseek error");
    	if (read(fd, &currentNode, sizeof(node)) != sizeof(node)) err(1, "Read error");

    	if (currentNode.next == 0) break;

    	currentIndex = currentNode.next;

	}
	
	if(lseek(fd, 0, SEEK_SET) < 0) err(1, "Lseek error");

	for(int i = 0; i < countOfNodes; i++)
	{
		
		if(!visited[i])
		{

			if(lseek(fd, i * sizeof(node), SEEK_SET) < 0) err(1, "Lseek error");
			uint8_t buffer[512] = {0};
			if(write(fd, buffer, sizeof(buffer)) < 0) err(1, "Write error");

		}

	}
	
	free(visited);
	close(fd);
	return 0;

}
