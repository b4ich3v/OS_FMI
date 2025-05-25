#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{

    if(argc < 3) errx(1, "Error");
    
    int duration = strtol(argv[1], NULL, 10);
    char* nameOfProgram = argv[2];
    char** arguments = &argv[2];

    int fd = open("run.log", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if(fd < 0) err(1, "Error");

    int prevStartTime = 0;
    bool prevFailed = false;

    while(true)
    {

        time_t currentStartTime = time(NULL);
        bool currentFailed = false;

        pid_t currentPid = fork();
        if(currentPid < 0) err(1, "Error");

        if(currentPid == 0)
        {

            execvp(nameOfProgram, arguments);
            err(1, "Error");

        }

        int status = 0;
        wait(&status);
        int currentExitStats = WEXITSTATUS(status);
        time_t currentEndTime = time(NULL);

        if(!WIFEXITED(status) || currentExitStats != 0) currentFailed = true;
        if ((prevFailed && currentFailed) && (currentStartTime - prevStartTime < duration)) break;
        if(currentFailed) { prevStartTime = currentStartTime; prevFailed = true; }
        else prevFailed = false; 
            
        dprintf(fd, "%ld %ld %d\n", currentStartTime, currentEndTime, currentExitStats);

    }

    close(fd);
    exit(0);

}

