#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

void print_per_senond(int sig) {
    printf("tick tock.\n");
}

int main() {
    
    struct itimerval it;
    it.it_interval.tv_sec = 1; //设置间隔 1 秒的定时器
    it.it_interval.tv_usec = 0;
    it.it_value = it.it_interval; //初始值设为和间隔相同（如果为 0 不会开始计时）

    signal(SIGALRM, &print_per_senond); //指定捕捉 alrm 信号
    if (-1 == setitimer(ITIMER_REAL, &it, NULL)) { //初始化定时器，开始计时
        perror("setitimer error");
        exit(1);
    }


    while(1) sleep(1); //让程序一直睡，不会阻塞到我们定义的回调
    return 0;
}