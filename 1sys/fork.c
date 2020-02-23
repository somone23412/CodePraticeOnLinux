#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main() {
    printf("main start.\n");
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error");
        exit(1);
    } else if (pid == 0) {
        printf("I am child progress, pid=%u, ppid=%u.\n", getpid(), getppid());
    } else {
        printf("I am parent, pid = %u, ppid = %u.\n", getpid(), getppid());
        sleep(1); //不加sleep父进程会提前结束，子进程的ppid会变成1
    }
    printf("main end.\n");
    return 0;
    //[output]
    // main start.
    // I am parent, pid = 40635, ppid = 25910.
    // main end.
    // I am child progress, pid=40636, ppid=40635.
    // main end.

    //[bash]
    // quyan@DGX-1-1:~/linuxc/1sys$ ps aux | grep 25910
    // quyan    25910  0.0  0.0  29984  5836 pts/16   Ss   15:23   0:00 /bin/bash
    
    //可以看到，父进程的父进程是bash
}