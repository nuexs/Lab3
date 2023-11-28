#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <command>, [argv0], [argv1]\n", argv[0]);
        exit(1);
    }
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        execvp(argv[1], argv + 1);
        perror("execvp failed:(");
    }
    else if (pid > 0)
    {
        wait((int *)0);
        exit(0);
    }
    else
        perror("fork failed");
}
