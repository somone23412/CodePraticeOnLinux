#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd = open("ps_out", O_WRONLY | O_CREAT, 0664);
    //判断是否打开...


    pid_t pid = fork();
    if (-1 == pid) {
        perror("fork");
        exit(1);
    } else if ( 0 == pid) {
        //把标准输出重定向到文件
        dup2(fd, STDOUT_FILENO);
        execl("/bin/ps", "ps", "-aux", NULL);
    } else {
        sleep(5);
        printf("parent\n");
    }
    //close(fd);
    return 0;
}