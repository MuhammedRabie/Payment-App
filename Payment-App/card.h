#pragma once
#ifndef CARD_H //header file guard
#define CARD_H
//typedefs
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef enum cardError {
	VALID,
	INVALID_NAME,
	INVALID_DATE,
	INVALID_PAN
}EN_CARD_ERROR_T; //
typedef struct CARD {
	//Card data
	uint8_t cardHolderName[25];//owner of the card name takes [20-24] chars
	uint8_t PAN[17];//Primary account number for this card.
	uint8_t cardExpirationDate[6]; // [MM/YY].
}ST_CARD_T;
EN_CARD_ERROR_T getCardHolderName(const ST_CARD_T* card);
EN_CARD_ERROR_T getCardExpiryDate(const ST_CARD_T* card);
EN_CARD_ERROR_T getCardPAN(const ST_CARD_T* card);
#endif