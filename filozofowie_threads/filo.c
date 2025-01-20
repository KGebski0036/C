#include "filo.h"

int main(void)
{

    start_symulation();

    return 0;
}

void start_symulation()
{

    pthread_t philosophers[NUM_PHILOSOPHERS];

    sem_t *tmp_forks = (sem_t *)malloc(NUM_PHILOSOPHERS * (sizeof(sem_t)));
    char *tmp_filo_status = (char *)calloc(NUM_PHILOSOPHERS + 1, 1);

    if (tmp_forks == 0 || tmp_filo_status == 0)
        printError("Nie udao sie zaalokowac pamieci");

    tmp_filo_status[NUM_PHILOSOPHERS] = 0;

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        sem_init(&(tmp_forks[i]), 0, 1);

    struct filo_data_s filo_data[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        filo_data[i].filo_id = i;
        filo_data[i].forks = tmp_forks;
        filo_data[i].filo_status = tmp_filo_status;
        if (pthread_create(&philosophers[i], NULL, philosopher_rutine, &(filo_data[i])))
            printError("Nie udalo sie utworzyc watku");
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(philosophers[i], NULL);
    }
}
