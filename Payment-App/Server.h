#pragma once
#ifndef SERVER_H
#define SERVER_H
#include <stdlib.h>
#include "terminal.h"
typedef struct ST_ACCOUNT_T {
	char PAN[17];
	float balance;
}ST_ACCOUNT_T;

typedef enum EN_serverError_t
{
	OK_serverError, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef struct ST_transaction_t
{
	ST_CARD_T cardHolderData;
	ST_TERMINAL_DATA_T terminalData;
	EN_transState_t transState;
}ST_transaction_t;

void updateBalance(char* PAN, float newbalance);
EN_transState_t recieveTransactionData(ST_transaction_t*);
EN_serverError_t isValidAccount(ST_CARD_T*);
EN_serverError_t isAmountAvailable(ST_TERMINAL_DATA_T*);
EN_serverError_t saveTransaction(ST_transaction_t*);
EN_serverError_t getTransaction(uint32_t, ST_transaction_t*);

#endif