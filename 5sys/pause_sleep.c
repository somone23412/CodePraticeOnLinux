#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void m_catch_alarm(int sig) {
    printf("alarm catched.\n");
}

unsigned int m_sleep(int sec) {
    // 如果不捕捉 alrm 信号，pause 就不会返回，程序会直接停止
    struct sigaction act, oldact; //定义捕捉动作
    act.sa_handler = &m_catch_alarm;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, &oldact); 

    alarm(sec); //用 alarm 定时，到点 pause 就会收到SIGALRM信号
    //-----------如果系统在这里失去了 cpu，卡了很久，还没调用到 pause，信号就已经发出去了，那样的话，等到调用 pause 时就无法退出了，所以要想办法解决时序竞态问题（比如在 alarm 前设置屏蔽 sigalrm 信号，在 pause 前设置【解除屏蔽 sigalrm 信号并立即执行 pasue】，就可以保证让 pause 收到信号，但这个“解除之后立即执行”的操作不是我们能定义的，所以需要其他函数来实现）
    int ret_p = pause(); //阻塞当前进程，等待闹钟信号
    printf("pause end with %d, continue.\n", ret_p);

    int ret = alarm(0); //就算被其它的信号吵醒也取消闹钟，返回剩余的秒数
    //解除信号捕捉动作
    sigaction(SIGALRM, &oldact, NULL); 

    return ret;
}

int main () {
    while(1) {
        m_sleep(2);
    }

    return 0;
}

// [OUTPUT]

// alarm catched.
// pause end with -1, continue.
// [2s]
// alarm catched.
// pause end with -1, continue.
// alarm catched.
// pause end with -1, continue.
// alarm catched.
// pause end with -1, continue.
// alarm catched.
// pause end with -1, continue.
// alarm catched.
// pause end with -1, continue.
// ...