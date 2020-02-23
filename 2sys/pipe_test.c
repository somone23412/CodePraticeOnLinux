#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>

int main(){
    int fds[2];
    int ret = pipe(fds);
    printf("father fd0 = %d, fd1 = %d\n", fds[0], fds[1]);
    if (-1 == ret) {
        perror("pipe error");
        exit(1);
    }
    pid_t pid = fork();
    if (pid == 0) {
        printf("child fd0 = %d, fd1 = %d\n", fds[0], fds[1]);
        // child read
        close(fds[1]);
        char buf[2] = {0};
        int ret;
        while (ret = read(fds[0], buf, sizeof(buf))) {
            //输出从父进程读到的东西
            write(1, buf, ret);
        }
        printf("child read done.\n");
        close(fds[0]);
    } else if (pid > 0){
        sleep(3); //管道是会阻塞的，如果父进程没往里写，子进程读就会阻塞在那里等着读，所以不会因为父进程写得比子进程读的晚而出问题
        // parent write
        close(fds[0]);
        char buf[] = "hello pipe\n";
        write(fds[1], buf, sizeof(buf));
        sleep(3);
        close(fds[1]); //只有关了写端之后，读端的read才不会被空管道阻塞
        wait(NULL);
    } else {
        perror("fork error");
        exit(1);
    }
    return 0;
}

// [output]

// father fd0 = 3, fd1 = 4
// child fd0 = 3, fd1 = 4
// [3s，read阻塞等待write内容]
// hello pipe
// [3s，read阻塞等待写端关闭]
// child read done.