#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(void) {
    //返回的是 void 指针，但我们定义个 char 指针存结果，就可以把这块内存用来存字符
    char *p = NULL;
    int fd = open("my_test.txt", O_CREAT|O_RDWR, 0644);
    if (fd < 0) {
        perror("file open");
        exit(1);
    }
    //把文件拓展为 4 字节的空洞文件
    int ret = ftruncate(fd, 4);
    if(ret == -1) {
        perror("ftruncate");
        exit(1);
    }

    //创建 4 个字节映射区，映射到 fd 空洞文件
    p = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap create");
        exit(1);
    }
    strcpy(p, "abcde\n"); //写数据，如果超过映射区长度，超出的部分就不写在文件里了

    printf("%c", *(p + 4)); //但是内存里的数据是完整的？

    //关 map
    int ret1 = munmap(p, 4);
    if (ret1 == -1) {
        perror("mmap close");
        exit(1);
    }
    close(fd);
    return 0;
}

// $./mmap_test 
// e
// $cat my_test.txt 
// abcd