#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/wait.h>

#include <string.h>


int main(void) {
    int *p;
    pid_t pid;
    int fd;

    fd = open("temp", O_RDWR | O_CREAT, 0664);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    unlink("temp"); //删除临时文件的硬链接（目录项 direnty），使其具【用完之后自动删除】
    ftruncate(fd, 4); //拓展空洞文件
    p = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //p = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd); //建立完映射区就可以关文件了

    pid = fork();
    if (pid == 0) {        //child
        sleep(5);
        p[0] = 666;

    } else if (pid > 0){        //parent
        for (int i = 0; i < 10; i++) {
            sleep(1);
            printf("p[0]=%d\n", p[0]);
        }
        wait(NULL);
        munmap(p, 4);
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}

// [output]
// p[0]=0
// p[0]=0
// p[0]=0
// p[0]=0
// p[0]=666
// p[0]=666
// p[0]=666
// p[0]=666
// p[0]=666
// p[0]=666