#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *foo(void *arg) {
    int i = (int)arg;
    sleep(i + 1);
    printf("child %d tid = %lu, pid = %u.\n", i + 1, pthread_self(), getpid());
    return NULL;
}

int main(void) {
    pthread_t tid;
    printf("parent original pid = %u.\n", getpid());
    int i;
    for (i = 0; i < 5; i++) {
        int ret = pthread_create(&tid, NULL, foo, (void *)i); //64 位系统 void * 8字节，可把 4 字节的 int 强转为 void*
        if (ret != 0) {
            fprintf(stderr, "pthread create error :%s", strerror(ret));
            exit(1);
        }
    }

    printf("parent tid = %lu, pid = %u.\n", pthread_self(), getpid());
    sleep(i + 1);
    return 0;
}

// [output]
// parent original pid = 17076.
// parent tid = 139691954616064, pid = 17076.
// child 1 tid = 139691946276608, pid = 17076.
// child 2 tid = 139691937883904, pid = 17076.
// child 3 tid = 139691929491200, pid = 17076.
// child 4 tid = 139691921098496, pid = 17076.
// child 5 tid = 139691912705792, pid = 17076.