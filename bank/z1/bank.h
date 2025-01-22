#ifndef BANK_H
# define BANK_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM_CUSTOMERS 10

struct key_to_bank_s
{
    int is_free;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
};

void printError(const char *str);

void start_symulation(void);
void* cusromer_rutine(void *);

void change_bank_account_money(int money);
void lockCriticalSection(struct key_to_bank_s* key);
void unlockCriticalSection(struct key_to_bank_s* key);

#endif