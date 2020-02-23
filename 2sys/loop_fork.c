#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc < 2){
        printf("./loop_fork 5.\n");
        exit(1);
    }

    pid_t pid = 0;
    int i;
    int n = atoi(argv[1]);
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) {
            //child
            break;
        } else if (pid > 0) {
            //parent
        } else {
            perror("fork error");
            exit(1);
        }
    }

    if (i == n) {
        //parent
        printf("I'm parent progress, pid=%d.\n", getpid());
        //循环回收所有子进程
        //while(waitpid(-1, NULL, 0)); //while(wait(NULL));
        
        //轮询回收所有子进程
        pid_t wpid;
        while (n > 0) {
            wpid = waitpid(-1, NULL, WNOHANG); //如果wpid == 0 说明子进程正在进行
            if (wpid > 0) n--;
            //do other things
            sleep(1);
        }
        while(1); //这里停住，另开终端看看后台有没有僵尸子进程
    } else {
        //child
        sleep(i);
        printf("I'm child progress %d, pid=%d, father=%d.\n", i, getpid(), getppid());
    }
    return 0;
}