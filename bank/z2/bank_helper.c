#include "bank.h"

extern int money_in_bank;

void printError(const char *str)
{
    printf("%s\n", str);
    exit(1);
}

void lockCriticalSection(struct key_to_bank_s* key, int money)
{
    pthread_mutex_lock(&(key->mutex));

    while (!key->is_free || (money_in_bank + money) < 0) {
        if (money < 0)
            pthread_cond_wait(&(key->normal_queue), &(key->mutex));
        else
            pthread_cond_wait(&(key->prior_queue), &(key->mutex));
    }

    key->is_free = 0;
    
    pthread_mutex_unlock(&(key->mutex));
}

void unlockCriticalSection(struct key_to_bank_s* key)
{
    pthread_mutex_lock(&(key->mutex));
    key->is_free = 1;
    pthread_cond_signal(&(key->prior_queue));
    pthread_cond_signal(&(key->normal_queue));
    pthread_mutex_unlock(&(key->mutex));
}