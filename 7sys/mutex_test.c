#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex;

void *foo(void *arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        write(1, "hello", 5);
        sleep(rand() % 3);
        write(1, "world\n", 6);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
    }
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_mutex_init(&mutex, NULL); // mutex == 1

    pthread_create(&tid, NULL, foo, NULL);
    pthread_detach(tid);

    while(1) {
        pthread_mutex_lock(&mutex); //lock 
        write(1, "HELLO", 5);
        sleep(rand() % 3);
        write(1, "WORLD\n", 6);
        pthread_mutex_unlock(&mutex); //unlock
        sleep(rand() % 3);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}