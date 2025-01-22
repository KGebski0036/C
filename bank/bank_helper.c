#include "bank.h"

extern int money_in_bank[2];

void printError(const char *str)
{
    printf("%s\n", str);
    exit(1);
}

void lockCriticalSection(struct key_to_bank_s *key, int account, int money)
{
    pthread_mutex_lock(&(key->mutex));

    while (!key->is_free[account] || (money_in_bank[account] + money) < 0)
    {
        if (money < 0)
            pthread_cond_wait(&(key->normal_queue[account]), &(key->mutex));
        else
            pthread_cond_wait(&(key->prior_queue[account]), &(key->mutex));
    }

    key->is_free[account] = 0;

    pthread_mutex_unlock(&(key->mutex));
}

void unlockCriticalSection(struct key_to_bank_s *key, int account)
{
    pthread_mutex_lock(&(key->mutex));
    key->is_free[account] = 1;
    pthread_cond_signal(&(key->prior_queue[account]));
    pthread_cond_signal(&(key->normal_queue[account]));
    pthread_mutex_unlock(&(key->mutex));
}