#ifndef BANK_H
#define BANK_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM_CUSTOMERS 10
#define NUM_TRANSFERS 4

extern int money_in_bank[2];

struct key_to_bank_s
{
    int is_free[2];
    pthread_mutex_t mutex;
    pthread_cond_t normal_queue[2];
    pthread_cond_t prior_queue[2];
};

void printError(const char *str);

void start_symulation(void);
void *cusromer_rutine(void *);
void *transfer_routine(void *val);

void change_bank_account_money(int money, int account);
void transfer_between_accounts(int from, int to, int amount);
void lockCriticalSection(struct key_to_bank_s *key, int account, int money);
void unlockCriticalSection(struct key_to_bank_s *key, int account);

#endif