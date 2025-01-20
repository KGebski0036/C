#ifndef FILO_H
#define FILO_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM_PHILOSOPHERS 5

void printError(const char *str);

void start_symulation();
void *philosopher_rutine(void *filo_data);

void eating(int filo_id, char *filo_status, sem_t *forks);
void meditating(int filo_id, char *filo_status);

struct filo_data_s
{
    int filo_id;
    sem_t *forks;
    char *filo_status;
};

#endif