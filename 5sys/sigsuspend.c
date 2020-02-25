#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void m_catch_alarm(int sig) {
    printf("alarm catched.\n");
}

unsigned int m_sleep(int sec) {
    // 如果不捕捉 alrm 信号，pause 就不会返回，程序会直接停止
    struct sigaction act, oldact; 
    sigset_t newmask, oldmask, tmpmask;
    unsigned int unslept;

    //1. 定义捕捉动作
    act.sa_handler = &m_catch_alarm;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, &oldact); 

    //2. 设置阻塞信号集，阻塞 sigalrm 信号
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask); //信号屏蔽字 mask

    //3. 定时，到时即发出信号，但信号会被阻塞住不会消失
    alarm(sec);

    //4. 构建一个 sigsupend 临时有效的阻塞信号集（注意是copy 一个 old，为了保持原本的其它屏蔽位），在其中解除对 sigalrm 的阻塞（因为有可能 old 本身就屏蔽了 sigalrm）
    tmpmask = oldmask;
    sigdelset(&tmpmask, SIGALRM);

    //5. sigsuspend 调用期间，采用的是临时信号阻塞集 susmask 替换原有的信号阻塞集
    // 这个信号集里sigalrm 没有被屏蔽，sigsuspend 挂起等待这个信号
    // 当 sigsuspend 被信号唤醒时，恢复采用原来的信号阻塞集
    int ret_s = sigsuspend(&tmpmask);
    printf("pause end with %d, continue.\n", ret_s);

    unslept = alarm(0); //就算被其它的信号吵醒也取消闹钟，返回剩余的秒数
    //6. sigsuspend 已经吃到了信号，恢复原本的屏蔽集
    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    //7. 解除信号捕捉动作
    sigaction(SIGALRM, &oldact, NULL); 
    return unslept;
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