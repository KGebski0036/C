#include "bank.h"

void printError(const char *str)
{
    printf("%s\n", str);
    exit(1);
}

void lockCriticalSection(struct key_to_bank_s* key)
{
    pthread_mutex_lock(&(key->mutex));

    while (!key->is_free) {
        pthread_cond_wait(&(key->cv), &(key->mutex));
    }

    key->is_free = 0;
    
    pthread_mutex_unlock(&(key->mutex));
}

void unlockCriticalSection(struct key_to_bank_s* key)
{
    pthread_mutex_lock(&(key->mutex));
    key->is_free = 1;
    pthread_cond_signal(&(key->cv));
    pthread_mutex_unlock(&(key->mutex));
}