#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <sys/types.h>
#include <sys/syscall.h>

#include <unistd.h>

#define __NR_custom_syscall 322

void* do_something(void *);

int main(int argc, char **argv)
{
	int thread_count = 0;
	int parent_pid;
	pthread_t pthread_array[1024];
	int thread_number_array[1024];
	int i, j;
	int thread_number;

	if(argc == 1)
	{
		printf("USAGE : ./thread_test [THREAD_COUNT]\n");
		return 0;
	}
	else
	{
		thread_count = atoi(argv[1]);
	}

	printf("****** Parent Process[PID : %6d | TID : %6d] ******\n", getpid(), (int)syscall(SYS_gettid));

	for(i = 0; i < thread_count; i++)
	{
		thread_number_array[i] = i + 1;
		pthread_create((&pthread_array[i]), NULL, do_something, &(thread_number_array[i]));
	}

	for(i = 0; i < thread_count; i++)
	{
		pthread_join(pthread_array[i], NULL);
	}

	usleep(10);

	printf("Total energy consumption: %d J\n", (int)syscall(__NR_custom_syscall, (int)getpid()));
	return 0;
}

void* do_something(void* thread_number)
{
	int* tnumber = thread_number;
	unsigned int i, j;
	int seed = 0;
	printf("*(%d) Child thread [PID : %6d | TID : %6d] starts the workload\n", (*tnumber), getpid(), (int)syscall(SYS_gettid));
	for(i; i < 2457664000; i++)
	{
		j = rand_r(&seed);
	}
	printf("*(%d) Child thread [PID : %6d | TID : %6d] ends the workload\n", (*tnumber), getpid(), (int)syscall(SYS_gettid));
	return;
}
