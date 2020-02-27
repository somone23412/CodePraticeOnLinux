#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5

int queue[N]; //0-4循环队列
sem_t blank_number, product_number;

void *p(void *arg) {
    int i = 0; //如果有多个生产者其实应该把 i 作为全局变量（消费者同理，但不是和生产者同一个 i）
    while(1) {
        sleep(rand() % 2);
        sem_wait(&blank_number);
//----------这一坨其实不严谨，要保证里面是原子操作（消费者同理）----------
        queue[i] = i; //producing
        printf("produced %d\n", i);
        i = (i + 1) % N;
//----------------------------------------------------
        sem_post(&product_number);
    }
}

void *c(void *arg) {
    int i = 0;
    while(1) {
        sleep(rand() % 5);
        sem_wait(&product_number);
        printf("consumed %d\n", queue[i]); //consuming
        i = (i + 1) % N;
        sem_post(&blank_number);
    }

}

int main() {
    pthread_t p_tid, c_tid;
    srand(time(NULL));
    sem_init(&blank_number, 0, N);
    sem_init(&product_number, 0, 0);
    pthread_create(&p_tid, NULL, p, NULL);
    pthread_create(&c_tid, NULL, c, NULL);
    pthread_join(p_tid, NULL);
    pthread_join(c_tid, NULL);
    sem_destroy(&blank_number);
    sem_destroy(&product_number);
    return 0;
}


//设定生产速度远大于消费速度，我们可以看到一上来就生产到了极限（4），之后只能每消费一个才能再生产一个
// quyan@DGX-1-1:~/linuxc/7sys$ ./sem_producer_consumer 
// produced 0
// produced 1
// produced 2
// produced 3
// produced 4
// consumed 0
// produced 0
// consumed 1
// produced 1
// consumed 2
// produced 2