#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void c_response(int sig) {
    printf("Ctrl + C detected!\n");
    sleep(5);
}


int main(void) {
    struct sigaction act; //建一个act 结构体

    act.sa_handler = c_response; //挂上捕捉处理函数
    sigemptyset(&act.sa_mask); //给一个空 mask
    sigaddset(&act.sa_mask, SIGQUIT); //在捕捉处理函数里屏蔽 Ctrl + \ 
    act.sa_flags = 0; //flag 设 0，默认在捕捉处理函数内部屏蔽目标信号
    act.sa_restorer = NULL; //废弃
    
    int ret = sigaction(SIGINT, &act, NULL); //捕捉 Ctrl + C中断，这里就不保存原来的 sigaction 了
    if (ret < 0) {
        perror("sigaction error");
        exit(1);
    }

    while(1);
    return 0;
}

//[output]
//^CCtrl + C detected!
// ^\^\^C^C^CCtrl + C detected! #处理函数运行时 c q 都被屏蔽
// ^C^C^C^C^C^CCtrl + C detected!
// ^\^\^\^C^C^CCtrl + C detected!
// ^\Quit #处理函数不在运行时可以 q 出来