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

int findIndex(pid_t pid, int* arr, int size)
{

    for (int i = 0; i < size; i++)
    {
        
        if(arr[i] == pid) return i;

    }
    
    return -1;

}

int main(int argc, char *argv[])
{

    if(argc < 2 || argc > 11) errx(1, "Error");

    int pids[10];

    for (int i = 1; i < argc; i++)
    {
        
        pid_t currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {

            execlp(argv[i], argv[i], (char*)NULL);
            err(1, "Error");

        }

        pids[i - 1] = currentPid;

    }
    
    ssize_t counter = argc - 1;
    ssize_t size = argc - 1;

    while(counter != 0)
    {

        int status = 0;
        pid_t currentPid = wait(&status);

        if(currentPid < 0) err(1, "Error");
        int pidIndex = findIndex(currentPid, pids, argc - 1);

        if(pidIndex == -1) continue;

        if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {

            counter -=1;
            pids[pidIndex] = -1;

        }
        else if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {

            pid_t newPid = fork();    
            if(newPid < 0) err(1, "Error");

            if(newPid == 0)
            {

                execlp(argv[pidIndex + 1], argv[pidIndex + 1], (char*)NULL);
                err(1, "Error");

            }

            pids[pidIndex] = newPid;

        }
        else
        {

            for (int i = 0; i < size; i++)
            {
                
                if(pids[i] != -1)
                {
                
                    kill(pids[i], SIGTERM);
                    if(waitpid(pids[i], NULL, 0) < 0) err(1, "Error");

                }

            }
            
            exit(pidIndex + 1);

        }

    }

    exit(0);

}
