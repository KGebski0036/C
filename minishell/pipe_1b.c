// Dodatkowo (1b), przetestować należy pojemność strumienia pipe, poprzez
// takie pokierowanie procesami (np. z wykorzystaniem funkcji sleep()), aby
// proces odbierający dane został na chwilę wstrzymany, podczas gdy proces
// wysyłający dane zapisuje je nieprzerwanym strumieniem.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void child(int fd[2])
{
    close(fd[0]);
    int limit = 0;
    for (;;)
    {
        limit += write(fd[1], "A", 1);
        printf("W bufforze jest: %d bajtow\n", limit);
    }
    close(fd[1]);
}

void parent(int fd[2])
{
    close(fd[1]);
    sleep(5);
    close(fd[0]);
}

int main(void)
{
    int fd[2];

    if (pipe(fd) < 0)
    {
        printf("Nie udao sie utworzyc strumienia");
        exit(1);
    }

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        printf("Error");
        break;
    case 0:
        child(fd);
        break;
    default:
        parent(fd);
        break;
    }
    return 0;
}
