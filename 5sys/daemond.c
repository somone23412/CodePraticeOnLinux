#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

void mydaemond() {
    //fork a child
    pid_t pid = fork();

    //重点是理解守护进程，就不检查出错了
    
    //parent exit
    if (pid > 0) exit(1);

    //create session
    pid_t sid = setsid();

    //change dir
    chdir("~");

    //file maske
    umask(0002); //664

    //std > /dev/null
    int fd = open("/dev/null", O_RDWR);
    for(int i = 0; i < 3; i++) 
        //if (i != 1) //to use printf
            dup2(fd, i);

    //do my work
    while(1) {
        sleep(5);
        printf("sid=%d pid=%d, ppid=%d, daemond is working \n", getsid(0), getpid(), getppid());
    }
}


int main(void) {
    mydaemond();
    return 0;
}