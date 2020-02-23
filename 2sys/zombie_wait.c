#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("I'm child, pid=%d, going to sleep 5s.\n", getpid());ew
        sleep(5);
        //int a = 5 / 0; 模拟程序异常退出
        printf("=======================child die =======================\n");
        return 100;

    } else if (pid > 0) {
        int stat_loc;  //获取子进程死亡状态，不需要获取就给wait传NULL，用宏函数判断子进程是怎么结束的
        pid_t wpid = wait(&stat_loc);
        if (wpid == -1) {
            perror("wait error");
            exit(1);
        } 
        printf("Meet child %d end, ", wpid);
        if (WIFEXITED(stat_loc)){
            printf("child normal exit, status = %d.\n", WEXITSTATUS(stat_loc));
            exit(1);
        } 
        if (WIFSIGNALED(stat_loc)){
            printf("child exit unexpectly, exit singal = %d.\n", WTERMSIG(stat_loc));
            exit(1);
        }

    } else {
        perror("fork error");
        exit(1);
    }
    return 0;
}