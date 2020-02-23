#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("error! ./a.out 5\n");
        exit(1);
    }
    printf("pid %u main start.\n", getpid());
    int i = 0;
    pid_t pid;

    //----------------------------------------
    for (; i < atoi(argv[1]); i++) {
        pid = fork();
        if (-1 == pid) {
            printf("error in fork %d", i);
            perror("");
            exit(1);
        } else if (0 == pid) {
            break; //进入子进程之后要break出循环
        }
    }
    //----------------------------------------
    sleep(i); //为了显示好看，越后创建的sleep越长
    if (0 == pid) {
        printf("I'am child progress%d, pid = %u, ppid = %u, i have done sth.\n", i, getpid(), getppid());
    } else {
        printf("I'am parent progress, pid = %u, ppid = %u, i've done sth.\n", getpid(), getppid());
    }
    return 0;
}

// [output]
// pid 42596 main start.
// I'am child progress0, pid = 42597, ppid = 42596, i have done sth.
// I'am child progress1, pid = 42598, ppid = 42596, i have done sth.
// I'am child progress2, pid = 42599, ppid = 42596, i have done sth.
// I'am child progress3, pid = 42600, ppid = 42596, i have done sth.
// I'am child progress4, pid = 42601, ppid = 42596, i have done sth.
// I'am parent progress, pid = 42596, ppid = 25910, i've done sth.