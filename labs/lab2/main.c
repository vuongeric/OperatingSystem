#include "account.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int acct, amt, choice = 0;

    while(choice == 0) {
        printf("Account # (0-99): ");
        scanf("%d", &acct);

        printf("Amount $ (>0): ");
        scanf("%d", &amt);

        deposit(amt, acct);
        printf("Balance in acct # %d is %d\n", acct, balance(acct));
        printf("Test: %d\n", balances[acct]);
        printf("Do this again? (0=yes, 1=no): ");
        scanf("%d", &choice);
    }
}