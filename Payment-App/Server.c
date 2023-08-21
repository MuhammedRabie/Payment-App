#ifndef SERVER_C
#define SERVER_C
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "Server.h"
#include "card.h"
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "terminal.h"

void loadAccountDB(ST_ACCOUNT_T* accounts, size_t maxAccounts) {
    FILE* accountFile = fopen("accounts.txt", "r"); //read file
    if (accountFile != NULL) {
        size_t i = 0;
        while (i < maxAccounts && fscanf(accountFile, "%s %f", accounts[i].PAN, &accounts[i].balance) == 2) {
            i++;
        }
        fclose(accountFile);
        printf("Account database loaded.\n");
    }
    else {
        printf("Error opening account database file.\n");
    }
}

void updateBalance(char* PAN, float newBalance) {
    FILE* accountFile = fopen("accounts.txt", "r"); //read from accounts.txt
    if (accountFile != NULL) {
        FILE* tempFile = fopen("temp_accounts.txt", "w"); //write to temp_accounts.txt
        if (tempFile != NULL) {
            ST_ACCOUNT_T account;
            while (fscanf(accountFile, "%16s %f\n", account.PAN, &account.balance) == 2) //extract PAN,Balance to tempaccount {
                if (strcmp(account.PAN,PAN) == 0) { //if pan matches update balance
                    fprintf(tempFile, "%s %.2f\n", account.PAN, newBalance);
                }
                else { //pan doesnt match , balance stays as it is.
                    fprintf(tempFile, "%s %.2f\n", account.PAN, account.balance);
                }
            }
            fclose(accountFile);
            fclose(tempFile);
            remove("accounts.txt");
            rename("temp_accounts.txt", "accounts.txt");
            printf("Account balance updated.\n");
        }
        else {
            fclose(accountFile);
            printf("Error opening temporary account database file.\n");
        }
}

ST_ACCOUNT_T* getAccountByPAN(const char* PAN) {
    FILE* accountFile = fopen("accounts.txt", "r");
    if (accountFile != NULL) {
        static ST_ACCOUNT_T tempAccount;
        while (fscanf(accountFile, "%s %f", tempAccount.PAN, &tempAccount.balance) == 2) {
            if (strcmp(tempAccount.PAN, PAN) == 0) {
                fclose(accountFile);
                return &tempAccount;
            }
        }
        fclose(accountFile);
    }
    return NULL;
}

EN_serverError_t isAmountAvailable(ST_TERMINAL_DATA_T* terminalData, ST_CARD_T* card) {
    //Checks if Balance is enough for transaction
    ST_ACCOUNT_T* account = getAccountByPAN(card->PAN);
    if (account != NULL && account->balance >= terminalData->transAmount) {
        return OK_serverError;
    }
    else {
        return LOW_BALANCE;
    }
}

EN_serverError_t isValidAccount(ST_CARD_T* card) {
    //Checks if PAN is in Database
    ST_ACCOUNT_T* account = getAccountByPAN(card->PAN);
    if (account != NULL) {
        return OK_serverError;
    }
    else {
        return ACCOUNT_NOT_FOUND;
    }
}

EN_transState_t recieveTransactionData(ST_transaction_t* transaction) {
    //Returns Transaction State.
    EN_serverError_t accountValidation = isValidAccount(&transaction->cardHolderData);
    if (accountValidation != OK_serverError) {
        return DECLINED_STOLEN_CARD;
    }

    EN_serverError_t amountAvailability = isAmountAvailable(&transaction->terminalData, &transaction->cardHolderData);
    if (amountAvailability != OK_serverError) {
        return DECLINED_INSUFFECIENT_FUND;
    }

    transaction->transState = APPROVED;
    return APPROVED;
}

const char* transStateToString(EN_transState_t state) {
    //helper function to convert transaction state to string to be stored in database.
    switch (state) {
    case APPROVED:
        return "APPROVED";
    case DECLINED_INSUFFECIENT_FUND:
        return "DECLINED_INSUFFECIENT_FUND";
    case DECLINED_STOLEN_CARD:
        return "DECLINED_STOLEN_CARD";
    case INTERNAL_SERVER_ERROR:
        return "INTERNAL_SERVER_ERROR";
    default:
        return "UNKNOWN";
    }
}

EN_serverError_t saveTransaction(ST_transaction_t* transaction) {
    //Saves Approved Transactions.
    if (transaction->transState != APPROVED) 
    {
        printf("Transaction rejected: ");
        switch (transaction->transState) {
        case DECLINED_INSUFFECIENT_FUND:
            printf("Insufficient funds.\n");
            break;
        case DECLINED_STOLEN_CARD:
            printf("Stolen card.\n");
            break;
        case INTERNAL_SERVER_ERROR:
            printf("Internal server error.\n");
            break;
        default:
            printf("application will terminate.\n");
            return 0;
        }
    }
    FILE* transactionFile = fopen("transactions.txt", "a");
    if (transactionFile != NULL) {
        fprintf(transactionFile, "%s %s %.6f %s\n",
            transaction->cardHolderData.PAN,
            transaction->terminalData.transactionDate,
            transaction->terminalData.transAmount,
            transStateToString(transaction->transState));

        fclose(transactionFile);
        return OK_serverError;
    }
    else {
        printf("Error opening transaction database file.\n");
        return SAVING_FAILED;
    }
}


#endif
