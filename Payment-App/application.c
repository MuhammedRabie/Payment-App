#ifndef APPLICATION_C
#define APPLICATION_C
#include"application.h"

void appStart()
{
    ST_CARD_T card;
    ST_TERMINAL_DATA_T terminal;
    ST_transaction_t transaction;
    ST_ACCOUNT_T account;

    // Get card information
    if (getCardHolderName(&card) != VALID) {
        printf("Invalid Card Holder Name!\n");
        return 0;
    }
    if (getCardExpiryDate(&card) != VALID)
    {
        printf("Invalid expiry date!\n)");
        return 0;
    }
    if (getCardPAN(&card) != VALID) {
        printf("INVALID PAN!\n");
        return 0;
    }
    // Get transaction information
    getTransactionDate(&terminal);
    if (isCardExpired(&terminal, &card) != OK_terminalError) {
        printf("Declined, Expired Card!\n");
        return 0;
    }

    setMaxAmount(&terminal);
    getTransactionAmount(&terminal);
    if (isBelowMaxAmount(&terminal) == EXCEED_MAX_AMOUNT) {
        printf("Declined Exceeding Max Transaction Amount!\n");
        return 0;
    }

    //Get Account information
    strcpy(account.PAN, card.PAN);

    // Populate transaction data
    transaction.cardHolderData = card;
    transaction.terminalData = terminal;

    EN_serverError_t transSt = recieveTransactionData(&transaction);
    if (transSt == DECLINED_STOLEN_CARD) {
        printf("Invalid Account!\n");
        return 0;
    }
    else if (transSt == DECLINED_INSUFFECIENT_FUND) {
        printf("Insufficient funds!\n");
        return 0;
    }
    else {
        printf("Approved transaction!\n");
        ST_ACCOUNT_T* x= getAccountByPAN(transaction.cardHolderData.PAN);
        float newBalance = x->balance - transaction.terminalData.transAmount;
        updateBalance(x->PAN, newBalance);
        printf("Account Updated!\nNew Balance is:  %f\n", newBalance);
        printf("Saving transaction..\n");
        if (saveTransaction(&transaction) == OK_serverError) {
            printf("Transaction Saved!\n");
        }
        
    }
}
#endif