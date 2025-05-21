#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{

    if(argc != 4) errx(1, "Error");

    int fd = open(argv[3], O_WRONLY | O_APPEND | O_CREAT, 0644);
    pid_t *pidsArray = malloc((argc - 2) * sizeof(pid_t));


    int trashDescriptor = open("/dev/null", O_WRONLY);
    int actaulOutDescriptor = dup(STDOUT_FILENO);
    dup2(trashDescriptor, STDOUT_FILENO);

    for (int i = 1; i < argc - 1; i++)
    {
        
        pid_t currentPid = fork();
        pidsArray[i - 1] = currentPid;

        if(currentPid == 0)
        {

            execlp(argv[i], argv[i], (char*) NULL);
            exit(1);

        }


    }

    for (int i = 0; i < argc - 2; i++)
    {
        
        int currentStatus = 0;
        char fictiveVariable = ' ';
        waitpid(pidsArray[i], &currentStatus, 0);

        if(WIFEXITED(currentStatus) && WEXITSTATUS(currentStatus) == 0)
        {

            if((write(fd, argv[i + 1], strlen(argv[i + 1])) < 0) ||
                (write(fd, &fictiveVariable, sizeof(char)) < 0)) 
            {

                close(fd);
                free(pidsArray);
                err(1, "Error");

            }

        }

    }
    
    free(pidsArray);
    close(fd);
    exit(0);

}
