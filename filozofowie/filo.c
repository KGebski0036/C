#include "filo.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
        printError("Program przyjmuje 2 argumenty. ./filo [numer_filozofa]");

    int filo_id = atoi(argv[1]);
    if (filo_id < 0 || filo_id > NUM_PHILOSOPHERS)
        printError("Błędny numer filozofa.");

    int shmid_status;
    int shmid_forks;

    if (filo_id == 0)
    {
        shmid_status = shmget(IPC_KEY, NUM_PHILOSOPHERS + 1, IPC_CREAT | IPC_EXCL | 0600);
        shmid_forks = shmget(IPC_KEY + 1000, NUM_PHILOSOPHERS, IPC_CREAT | IPC_EXCL | 0600);

        if (shmid_status == -1 || shmid_forks == -1)
            printError("Nie można utworzyć pamięci współdzielonej");
    }
    else
    {
        shmid_status = shmget(IPC_KEY, NUM_PHILOSOPHERS + 1, 0600);
        shmid_forks = shmget(IPC_KEY + 1, NUM_PHILOSOPHERS, 0600);
    }

    char *filo_status = (char *)shmat(shmid_status, NULL, 0);
    sem_t *forks = (sem_t *)shmat(shmid_status, NULL, 0);

    if (filo_status == (void *)-1 || forks == (void *)-1)
        printError("Nie można dostać dostępu pamięci współdzielonej");

    if (filo_id == 0)
    {
        filo_status[NUM_PHILOSOPHERS] = '\0';

        for (int i = 0; i < NUM_PHILOSOPHERS; i++)
            sem_init(&forks[i], 0, 1);

        observator(filo_status);
    }
    else
    {
        philosopher(filo_id, filo_status, forks);
    }

    shmdt(filo_status);

    if (filo_id == 0)
        shmctl(shmid_status, IPC_RMID, NULL);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_destroy(&forks[i]);
    }

    return 0;
}

void observator(char *filo_status)
{
    printf("Status filozofow:\n");
    char *last_status = "-----";
    printf("%s\n", filo_status);
    while (strcmp(filo_status, "ddddd") != 0)
    {

        if (strcmp(filo_status, last_status) != 0)
        {
            printf("%s\n", filo_status);
            strcpy(filo_status, last_status);
        }

        usleep(5000);
    }
}

void philosopher(int filo_id, char *filo_status, sem_t *forks)
{
    filo_id--;

    for (int i = 0; i < 7; i++)
    {
        meditating(filo_id, filo_status);

        eating(filo_id, filo_status, forks);
    }

    filo_status[filo_id] = 'd';
}
