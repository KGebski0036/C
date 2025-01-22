#include "bank.h"

int money_in_bank = 1000;
struct key_to_bank_s key;

int main(void)
{

    start_symulation();
    return 0;
}

void start_symulation(void)
{

    pthread_t customers[NUM_CUSTOMERS];

    key.is_free = 1;
    pthread_mutex_init(&(key.mutex), NULL);
    pthread_cond_init(&(key.cv), NULL);

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        if (pthread_create(&customers[i], NULL, cusromer_rutine, NULL))
            printError("Nie udalo sie utworzyc watku");
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        pthread_join(customers[i], NULL);
    }

    pthread_mutex_destroy(&(key.mutex));
    pthread_cond_destroy(&(key.cv));
}

void *cusromer_rutine(void *)
{

    for (int i = 0; i < 5; i++)
    {
        change_bank_account_money(-100);
        change_bank_account_money(100);
    }
    return 0;
}

void change_bank_account_money(int money)
{
    printf("Change money: %d\n", money);

    lockCriticalSection(&key);

    int tmp = money_in_bank;
    usleep((rand() % 5 + 1) * 50000);
    money_in_bank = tmp + money;

    unlockCriticalSection(&key);

    printf("Account after change: %d\n", money_in_bank);
}
