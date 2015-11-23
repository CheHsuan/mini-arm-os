#ifndef __THREADS_H__
#define __THREADS_H__

typedef struct{
	char *name;
	unsigned int priority;
	volatile int *mutexlock;
} threadInfo;

void thread_start();
int thread_create(void (*run)(void*), void* userdata);
void thread_kill(int thread_id);
void thread_self_terminal();

#endif
