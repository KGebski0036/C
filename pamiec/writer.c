#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
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

    for (int i = 0; i < NUMBER_OF_MESSAGES; ++i)
    {
        shmp->i = i;
        strcpy(shmp->buf, "Hello World");
        sleep(1);
    }

    if (shmdt(shmp) == -1)
        printError("Coudnt detach shared memory");

    return 0;
}
