#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct products{
    struct products *next;
    int val;
} *headp = NULL;


//静态初始化
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  has_product = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int i = 0;
    while(1) {
        struct products* mp = (struct products*) malloc(sizeof(struct products));
        mp->val = i++; //模拟一个产品
        printf("-Produce ---%d\n", mp->val);

        pthread_mutex_lock(&mutex);
        mp->next = headp;
        headp = mp;
        pthread_mutex_unlock(&mutex);

        pthread_cond_broadcast(&has_product); //给在 wait 的线程发信号
        sleep(rand() % 2);
    }
    return NULL;
}

void *consumer(void *arg) {
    while(1) {//消费者一直消费
        pthread_mutex_lock(&mutex); //首先要锁住
        while (headp == NULL) { //生产线上如果没有产品
            pthread_cond_wait(&has_product, &mutex); //上交锁，等信号，某一刻信号来了（有货），一个线程抢到锁继续往下执行，其它线程虽然也收到了信号，但是没拿到锁，就等着拿到的那个线程释放锁，等轮到的时候那就得再判断一次还有没有产品了，没有那就得再等下一个信号，所以用 while 循环
        }
        struct products* mp = headp; //拿走一个流水线上的产品
        headp = mp->next; 
        pthread_mutex_unlock(&mutex); //上交锁

        printf("-Consume ---%d\n", mp->val); //产品被拿走之后消费者想干嘛就干嘛了
        free(mp);
        sleep(rand() % 5);
    }
    return NULL;
}

int main(void) {
    pthread_t p_tid, c_tid1, c_tid2;
    srand(time(NULL)); //真随机

    pthread_create(&p_tid, NULL, producer, NULL); //一个生产者两个消费者
    pthread_create(&c_tid1, NULL, consumer, NULL);
    pthread_create(&c_tid2, NULL, consumer, NULL);

    pthread_join(p_tid, NULL);
    pthread_join(c_tid1, NULL);
    pthread_join(c_tid2, NULL);

    return 0;
}

// quyan@DGX-1-1:~/linuxc/7sys$ ./cond_producer_consumer 
// -Produce ---0
// -Consume ---0
// -Produce ---1
// -Consume ---1
// -Produce ---2
// -Consume ---2
// -Produce ---3
// -Produce ---4
// -Produce ---5
// -Produce ---6
// -Produce ---7
// -Consume ---7
// -Consume ---6
// -Consume ---5
// -Consume ---4