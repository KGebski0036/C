#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

#include "shared.h"

int main()
{
    int shmid = shmget(KEY, 1024, 0644 | IPC_CREAT);
    if (shmid == -1)
        printError("Coudnt create shared memory");

    Message *shmp = shmat(shmid, NULL, 0);

    if (shmp == (void *)-1)
        printError("Coudnt attach shared memory");

    int lastId = -1;
    while (lastId != NUMBER_OF_MESSAGES - 1)
    {
        if (lastId != shmp->i)
        {
            lastId = shmp->i;
            printf("id: %d; message: %s\n", shmp->i, shmp->buf);
        }
        sleep(0.2);
    }

    if (shmdt(shmp) == -1)
        printError("Coudnt detach shared memory");

    return 0;
}