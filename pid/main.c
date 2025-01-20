#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid = fork();
	switch (pid)
	{
	case -1:
		printf("Nie udalo sie stworzyc\n");
		exit(-1);
	case 0:
		printf("Jestem potomnym pid:%d ppid: %d\n", getpid(), getppid());
		exit(0);
	default:
		printf("Jestem rodzicem pid:%d ppid: %d\n", getpid(), getppid());
		wait(NULL);
	}
	return 0;
}
