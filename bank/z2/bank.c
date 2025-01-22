#include "bank.h"

int money_in_bank = 0;
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
    pthread_cond_init(&(key.normal_queue), NULL);
    pthread_cond_init(&(key.prior_queue), NULL);

    int task[NUM_CUSTOMERS];

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        task[i] = i % 2;
        if (pthread_create(&customers[i], NULL, cusromer_rutine, &(task[i])))
            printError("Nie udalo sie utworzyc watku");
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        pthread_join(customers[i], NULL);
    }

    pthread_mutex_destroy(&(key.mutex));
    pthread_cond_destroy(&(key.prior_queue));
    pthread_cond_destroy(&(key.normal_queue));
}

void *cusromer_rutine(void *val)
{

    int randomizer = *((int *) val);

    for (int i = 0; i < 5; i++)
    {
        change_bank_account_money(100 + (-200 * randomizer));
        change_bank_account_money(100 + (-200 * randomizer));
        change_bank_account_money(100 + (-200 * randomizer));
        change_bank_account_money(100 + (-200 * randomizer));
        change_bank_account_money(100 + (-200 * randomizer));
    }
    return 0;
}

void change_bank_account_money(int money)
{
    printf("Change money: %d\n", money);

    lockCriticalSection(&key, money);

    int tmp = money_in_bank;
    usleep((rand() % 5 + 1) * 50000);
    money_in_bank = tmp + money;

    unlockCriticalSection(&key);

    printf("Account after change: %d\n", money_in_bank);
}
