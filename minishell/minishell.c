#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void print_error(const char* err)
{
	printf("%s\n", err);
	exit(1);
}

void set_pipe(int has_pipe, int *fd, int end)
{
    if (has_pipe && (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
        print_error("FD duplication failed");
}

void exec(char **argv, int i)
{
    int has_pipe, fd[2], pid;

    has_pipe = argv[i] && !strcmp(argv[i], ".");

    if (has_pipe && pipe(fd) == -1)
		print_error("Pipe failed");

    if ((pid = fork()) == -1)
        print_error("Fork failed");

    if (!pid)
    {
        argv[i] = 0;
        set_pipe(has_pipe, fd, 1);

        execvp(*argv, argv);

        print_error("Execuson of program failed");
    }

    waitpid(pid, NULL, 0);

    set_pipe(has_pipe, fd, 0);
}

int main(int, char **argv)
{
    int i = 0;

    while (argv[i])
    {
        argv += i + 1;
        i = 0;
        while (argv[i] && strcmp(argv[i], "."))
            i++;
        if (i)
            exec(argv, i);
    }
    return 0;
}
