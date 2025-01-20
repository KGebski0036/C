
#ifndef FILO_H
#define FILO_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define IPC_KEY 3016
#define NUM_PHILOSOPHERS 5

void printError(const char *str);

void observator(char *status);
void philosopher(int tmp_id, char *status, sem_t *forks);

void eating(int filo_id, char *filo_status, sem_t *forks);
void meditating(int filo_id, char *filo_status);

#endif