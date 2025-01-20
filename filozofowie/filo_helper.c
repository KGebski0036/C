#include "filo.h"

void printError(const char *str)
{
    printf("%s\n", str);
    exit(1);
}

void meditating(int filo_id, char *filo_status)
{
    filo_status[filo_id] = 'm';
    printf("Filozof %d: [%c] -- medytuje\n", filo_id, filo_status[filo_id]);
    usleep((rand() % 5 + 1) * 500000);
}

void eating(int filo_id, char *filo_status, sem_t *forks)
{

    int left_fork = filo_id;
    int right_fork = (filo_id + 1) % NUM_PHILOSOPHERS;

    filo_status[filo_id] = 'w';
    printf("Filozof %d: [%c] -- czeka na widelec\n", filo_id, filo_status[filo_id]);

    while (1)
    {
        if (sem_trywait(&forks[left_fork]) == 0)
        {
            filo_status[filo_id] = 'l';
            printf("Filozof %d: [%c] -- ma lewy widelec\n", filo_id, filo_status[filo_id]);

            if (sem_trywait(&forks[right_fork]) == 0)
            {
                filo_status[filo_id] = 'e';
                printf("Filozof %d: [%c] -- posila się\n", filo_id, filo_status[filo_id]);
                usleep((rand() % 5 + 1) * 500000);

                sem_post(&forks[right_fork]);
                sem_post(&forks[left_fork]);
                break;
            }
            else
            {
                filo_status[filo_id] = 'w';
                printf("Filozof %d: [%c] -- oddał lewy widelec\n", filo_id, filo_status[filo_id]);
                sem_post(&forks[left_fork]);
            }
        }
        usleep((rand() % 5 + 1) * 500000);
    }

    filo_status[filo_id] = '-';
    printf("Filozof %d: [%c] -- skończył jeść\n", filo_id, filo_status[filo_id]);
}