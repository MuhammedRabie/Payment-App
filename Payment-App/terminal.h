#pragma once
#ifndef TERMINAL_H
#define TERMINAL_H
#include "card.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef enum 
{
	 WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT, OK_terminalError
}EN_TERMINALERROR_T;
typedef struct {
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_TERMINAL_DATA_T;

EN_TERMINALERROR_T getTransactionDate(ST_TERMINAL_DATA_T* terminalData);
EN_TERMINALERROR_T isCardExpired(ST_TERMINAL_DATA_T* terminalData, ST_CARD_T* card);
EN_TERMINALERROR_T getTransactionAmount(ST_TERMINAL_DATA_T* terminalData);
EN_TERMINALERROR_T setMaxAmount(ST_TERMINAL_DATA_T* terminalData);
EN_TERMINALERROR_T isBelowMaxAmount(ST_TERMINAL_DATA_T* terminalData);
#endif