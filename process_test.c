#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//#include <linux/random.h>

#include <sys/types.h>
#include <sys/wait.h>

#define __NR_custom_syscall 322

int main(int argc, char **argv)
{
	int process_count = 0;
	int parent_pid;
	int child_pid_array[1024];
	int fork_pid = 0;
	unsigned int i, j;
	int child_number;

	if(argc == 1)
	{
		printf("USAGE : ./process_test [PROCESS_COUNT]\n");
		return 0;
	}
	else
	{
		process_count = atoi(argv[1]);
	}

	printf("****** Parent Process[PID : %6d] ******\n", getpid());

	for(i = 0; i < process_count; i++)
	{
		child_number = i + 1;
		fork_pid = 0;
		fork_pid = fork();
		if(fork_pid == 0)
		{
			//i'm child
			break;
		}
		else
		{
			//i'm parent
			child_pid_array[i] = fork_pid;
		}
	}

	if(fork_pid == 0)
	{
		int seed = 0;
		//i'm child
		printf("*(%d) Child process [PID:%6d] starts the workload\n", child_number, getpid());
		for(i = 0; i < 2457664000; i++)
		{
			j = rand_r(&seed);
		}
		printf("*(%d) Child process [PID:%6d] ends the workload\n", child_number, getpid());
		return 1;
	}
	else
	{
		//i'm parent
		int status;
		for(i = 0; i < process_count; i++)
		{
			waitpid(child_pid_array[i], &status, 0);
		}
      
		usleep(10);
		
		printf("Total energy consumption: %d J\n", (int)syscall(__NR_custom_syscall, (int)getpid()));
		return 0;
	}
}
