#include <stdio.h>
#include <pthread.h>

int shared_i = 0;

void increase_num(void);

int main(){
    pthread_t thrd1, thrd2, thrd3;
    if (0 == pthread_create(&thrd1, NULL, (void *)increase_num, NULL)){
        printf("Create thrd1 success!\n");
    }
    if (0 == pthread_create(&thrd2, NULL, (void *)increase_num, NULL)){
        printf("Create thrd2 success!\n");
    }
    if (0 == pthread_create(&thrd1, NULL, (void *)increase_num, NULL)){
        printf("Create thrd3 success!\n");
    }
    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    pthread_join(thrd3, NULL);
    
    printf("shared_i = %d.\n", shared_i);

    return 0;
}

void increase_num(void){
    int tmp;
    for (int i = 0; i < 100000; i++){
        tmp = shared_i;
        tmp = tmp + 1;
        shared_i = tmp;
    }
}

/*
Running result:
    shared_i = 104999.
*/
