#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int shared_i = 0;

void increase_num(void);

pthread_mutex_t mutex;

int main(){
    pthread_t thrd1, thrd2, thrd3;

    pthread_mutex_init(&mutex, NULL);

    if (0 == pthread_create(&thrd1, NULL, (void *)increase_num, NULL)){
        printf("Create thrd1 success!\n");
    };
    if (0 == pthread_create(&thrd2, NULL, (void *)increase_num, NULL)){
        printf("Create thrd1 success!\n");
    };
    if (0 == pthread_create(&thrd1, NULL, (void *)increase_num, NULL)){
        printf("Create thrd1 success!\n");
    };

    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    pthread_join(thrd3, NULL);
    
    printf("shared_i = %d.\n", shared_i);

    return 0;
}

void increase_num(void){
    int tmp;
    for (int i = 0; i < 100000; i++){
        //lock
        if(0 != pthread_mutex_lock(&mutex)){
            perror("pthread_mutex_lock error");
            exit(EXIT_FAILURE);
        }
            tmp = shared_i;
            tmp = tmp + 1;
            shared_i = tmp;
        //unlock
        if(0 != pthread_mutex_unlock(&mutex)){
            perror("pthread_mutex_unlock error");
            exit(EXIT_FAILURE);
        }
    }
}

/*
Running result:
    shared_i = 300000.
*/
