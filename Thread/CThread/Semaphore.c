#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
/*
初始化信号量： int sem_init(sem_t *sem, int pshared, unsigned int value);
    成功返回0，失败返回-1
参数
    sem：指向信号量结构的一个指针
    pshared： 不是0的时候，该信号量在进程间共享，否则只能为当前进程的所有线程们共享
    value：信号量的初始值

信号量减1操作，当sem=0的时候该函数会堵塞: int sem_wait(sem_t *sem);
    成功返回0，失败返回-1
参数
    sem：指向信号量的一个指针

信号量加1操作 int sem_post(sem_t *sem);
参数与返回同上

销毁信号量 int sem_destroy(sem_t *sem);
参数与返回同上
*/



#define MAXSIZE 10

int stack[MAXSIZE];
int size = 0;
sem_t sem;

//provide
void provide_data(void) {
    for (int i = 0; i < MAXSIZE; i++) {
        stack[i] = i;
        printf("provide: %d.\n", i);
        sem_post(&sem);
        if( i >= 5) {
             sleep(1);
        }
    }
}

//custom
void handle_data(void) {
    int i;
    while ((i = size++) < MAXSIZE) {
        sem_wait(&sem); //If there is no product left, customor will wait provider to provide product.
        printf("custom: %d: %d X %d = %d.\n", stack[i], stack[i], stack[i], stack[i] * stack[i]);
    }
}

int main() {
    pthread_t provider, handler;

    sem_init(&sem, 0, 0);//init sem
    pthread_create(&provider, NULL, (void *)&provide_data, NULL);
    pthread_create(&handler, NULL, (void *)&handle_data, NULL);

    pthread_join(provider, NULL);
    pthread_join(handler, NULL);

    sem_destroy(&sem);//destroy sem



    return 0;
}



/*
Running result:
    provide: 0.
    provide: 1.
    provide: 2.
    provide: 3.
    provide: 4.
    provide: 5.
    custom: 0: 0 X 0 = 0.
    custom: 1: 1 X 1 = 1.
    custom: 2: 2 X 2 = 4.
    custom: 3: 3 X 3 = 9.
    custom: 4: 4 X 4 = 16.
    custom: 5: 5 X 5 = 25.
    provide: 6.
    custom: 6: 6 X 6 = 36.
    provide: 7.
    custom: 7: 7 X 7 = 49.
    provide: 8.
    custom: 8: 8 X 8 = 64.
    provide: 9.
    custom: 9: 9 X 9 = 81.
*/














