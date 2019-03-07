#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <poll.h>

int main(int argc, char** argv)
{
    if (argc <= 2) {
        printf("error param!\n");
        return 0;
    }
    int pipeFd[2] = {-1};
    pipe(pipeFd);
    pid_t pid = fork();
    if (pid == 0) {
        close(pipeFd[1]);
        dup2(pipeFd[0], STDIN_FILENO);
        close(pipeFd[0]);
        char* arg[] = {argv[1], NULL};
        if (execv(argv[1], arg) < 0) {
            printf("execv fail, error: %d, %s\n", errno, strerror(errno));
            return -1;
        }
        return 0;
    }
    else if (pid > 0) {
        close(pipeFd[0]);
        printf("pid: %d\n", pid);
        for (int i = 2; i < argc; ++i) {
            sleep(1);
            char data[1024] = {0};
            snprintf(data, sizeof(data), "%s\n\r", argv[i]);
            write(pipeFd[1], data, strlen(data));
        }
        waitpid(pid, NULL, 0);
        close(pipeFd[0]);
        close(pipeFd[1]);
        return 0;
    }
    else {
        printf("fuck fork!\n");
        return -1;
    }
}
