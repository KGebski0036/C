#include "bank.h"

int money_in_bank[2] = {100, 100};
struct key_to_bank_s key;

int main(void)
{
    srand(time(NULL));
    start_symulation();
    return 0;
}

void start_symulation(void)
{

    pthread_t customers[NUM_CUSTOMERS];
    pthread_t transfers[NUM_TRANSFERS];

    key.is_free[0] = 1;
    key.is_free[1] = 1;
    pthread_mutex_init(&(key.mutex), NULL);
    pthread_cond_init(&(key.normal_queue[0]), NULL);
    pthread_cond_init(&(key.prior_queue[0]), NULL);
    pthread_cond_init(&(key.normal_queue[1]), NULL);
    pthread_cond_init(&(key.prior_queue[1]), NULL);

    int task[NUM_CUSTOMERS];

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        task[i] = i % 2;
        if (pthread_create(&customers[i], NULL, cusromer_rutine, &(task[i])))
            printError("Nie udalo sie utworzyc watku");
    }

    int task_2[NUM_TRANSFERS];

    for (int i = 0; i < NUM_TRANSFERS; i++)
    {
        task_2[i] = i % 2;
        if (pthread_create(&transfers[i], NULL, transfer_routine, &(task_2[i])))
            printError("Failed to create transfer thread");
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        pthread_join(customers[i], NULL);
    }

    for (int i = 0; i < NUM_TRANSFERS; i++)
    {
        pthread_join(transfers[i], NULL);
    }

    pthread_mutex_destroy(&(key.mutex));
    pthread_cond_destroy(&(key.normal_queue[0]));
    pthread_cond_destroy(&(key.prior_queue[0]));
    pthread_cond_destroy(&(key.normal_queue[1]));
    pthread_cond_destroy(&(key.prior_queue[1]));
}

void *cusromer_rutine(void *val)
{

    int randomizer = *((int *)val);

    for (int i = 0; i < 5; i++)
    {
        change_bank_account_money(100 + (-200 * randomizer), 0);
        change_bank_account_money(100 + (-200 * randomizer), 1);
        change_bank_account_money(100 + (-200 * randomizer), 0);
        change_bank_account_money(100 + (-200 * randomizer), 1);
        change_bank_account_money(100 + (-200 * randomizer), 0);
        change_bank_account_money(100 + (-200 * randomizer), 1);
        change_bank_account_money(100 + (-200 * randomizer), 0);
        change_bank_account_money(100 + (-200 * randomizer), 1);
        change_bank_account_money(100 + (-200 * randomizer), 0);
        change_bank_account_money(100 + (-200 * randomizer), 1);
    }
    return 0;
}

void *transfer_routine(void *val)
{

    int randomizer = *((int *)val);

    int from = 1 * randomizer;
    int to = 1 - from;

    for (int i = 0; i < 2; i++)
    {

        transfer_between_accounts(from, to, 100);
        transfer_between_accounts(to, from, 200);
        transfer_between_accounts(from, to, 100);
    }
    return 0;
}

void change_bank_account_money(int money, int account)
{
    printf("Change money: %d, account: %d\n", money, account);

    lockCriticalSection(&key, account, money);

    int tmp = money_in_bank[account];
    usleep((rand() % 5 + 1) * 50000);
    money_in_bank[account] = tmp + money;

    unlockCriticalSection(&key, account);

    printf("Account[%d] after change: %d\n", account, money_in_bank[account]);
}

void transfer_between_accounts(int from, int to, int amount)
{

    change_bank_account_money(-amount, from);
    change_bank_account_money(amount, to);

    printf("Transferred %d from Account %d to Account %d. Balances: %d, %d\n", amount, from, to, money_in_bank[from], money_in_bank[to]);

}