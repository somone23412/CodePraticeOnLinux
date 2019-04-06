#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//compile command line: gcc ThreadCreate.c -std=gnu99 -l pthread -o ThreadCreate
//create thread : pthread_create(pthread_t * thread, const pthread_attr_t * attr, (void *)(*)(void *) start_routine, void * arg);
//exit thread : pthread_exit(void *value_ptr)
//join(wait) thread : pthread_join(pthread_t thread, void ** value_ptr)


int print_message_1(void *ptr);
int print_message_2(void *ptr);

int main() {
	void *retval;
	pthread_t thread1, thread2;
	char *message1 = "thread1";
	char *message2 = "thread2";

	if (0 == pthread_create(&thread1, NULL, (void *)&print_message_1, (void *)message1)) {
		printf("Create thread1 successed!\n");
	}else {
		printf("Create thread1 failed!\n");
	}
	if (0 == pthread_create(&thread2, NULL, (void *)&print_message_2, (void *)message2)) {
		printf("Create thread2 successed!\n");
	}else {
		printf("Create thread2 failed!\n");
	}

	if (0 != pthread_join(thread1, &retval)) {
		printf("Can not join with thread1.\n");
	}else {
		printf("Thread1 return value(retval) is %d.\n", (int)retval);
		printf("Thread1 end.\n");
	}

	if (0 != pthread_join(thread2, &retval)) {
		printf("Can not join with thread2.\n");
	}else {
		printf("Thread2 return value(retval) is %d.\n", (int)retval);
		printf("Thread2 end.\n");
	}

	if (0 != pthread_join(thread1, &retval)) {
		//Can not join thread 1 again.
		printf("Can not join with thread1.\n");
	}

	return 0;
}

int print_message_1(void *ptr){
	for (int i = 0; i < 5; i++) {
		printf("%s:%d\n", (char *)ptr, i);
	}
	return 1;
}

int print_message_2(void *ptr){
	for (int i = 0; i < 5; i++) {
		printf("%s:%d\n", (char *)ptr, i);
	}
	return 2;
}

/*
Running result:
	Create thread1 successed!
	Create thread2 successed!
	thread1:0
	thread1:1
	thread1:2
	thread2:0
	thread2:1
	thread1:3
	thread1:4
	thread2:2
	thread2:3
	thread2:4
	Thread1 return value(retval) is 1.
	Thread1 end.
	Thread2 return value(retval) is 2.
	Thread2 end.
	Can not join with thread1.
*/
