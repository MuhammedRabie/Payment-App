#ifndef TERMINAL_C
#define TERMINAL_C
#define _CRT_SECURE_NO_WARNINGS
#include "terminal.h"
#include <time.h>
#include <stdio.h>
#include<string.h>

int daysInMonth(int year, int month) {
    int daysPerMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))) {
        return 29; // Leap year
    }
    return daysPerMonth[month];
}
EN_TERMINALERROR_T getTransactionDate(ST_TERMINAL_DATA_T* terminalData)
{
	char ans = 's';
	int maxAttempts = 3;
	printf("Manual Transaction Date or Current System Date?\n[M for manual ,S for system date]: ");
	while (maxAttempts > 0) {
        if (maxAttempts < 3 && maxAttempts != 0) {
            printf("Re-enter your transaction date choice[M/S]: ");
        }
		int result=scanf(" %c", &ans); //space is to ignore any whitespaces before reading.
        char x=getchar();
		if (ans == 'S' || ans == 's') {
			printf("System is retreiving current date..\n");
			// Get current time in seconds
			time_t currentTime;
			time(&currentTime);
			// Convert time to a struct tm
			struct tm* localTime = localtime(&currentTime);
			// Format the date as YYYY-MM-DD
			strftime(terminalData->transactionDate, sizeof(terminalData->transactionDate), "%Y/%m/%d", localTime);
			printf("transactionDate: %s\n", terminalData->transactionDate);
            return OK_terminalError;
		}

        else if (ans == 'M' || ans == 'm') {
            printf("Enter Date with this format YYYY/MM/DD\n");
            uint8_t temp[11];

            if (fgets(temp, sizeof(temp), stdin) == NULL) { //Reads Date into temp
                printf("Error reading input!\n");
                return WRONG_DATE;
            }

            temp[strcspn(temp, "\n")] = '\0'; //replaces newline with '\0'

            if (strlen(temp) != 10 || temp[4] != '/' || temp[7] != '/') {
                printf("Invalid date format!\n");
                maxAttempts--;

                if (maxAttempts == 0) {
                    printf("Max Attempts Reached! Application will terminate.\n");
                    return WRONG_DATE;
                }
            }
            else {
                int year, month, day;
                if (sscanf(temp, "%4d/%2d/%2d", &year, &month, &day) != 3) {
                    printf("Invalid Date Format!\n");
                    maxAttempts--;
                    if (maxAttempts == 0) {
                        printf("Max Attempts Reached! Application will terminate.\n");
                        return WRONG_DATE;
                    }
                }
                else if (year < 0 || year > 9999 || month < 1 || month > 12 || day < 1 || day > daysInMonth(year, month)) {
                    printf("Invalid date!\n");
                    maxAttempts--;
                    if (maxAttempts == 0) {
                        printf("Max Attempts Reached! Application will terminate.\n");
                        return WRONG_DATE;
                    }
                }
                else {
                    snprintf(terminalData->transactionDate, sizeof(terminalData->transactionDate), "%04d/%02d/%02d", year, month, day);
                    printf("transactionDate: %s\n", terminalData->transactionDate);
                    return OK_terminalError;
                }
            }
        }
		else {
			maxAttempts--;
			if (maxAttempts == 0) {
				printf("No more attempts,Application will terminate!");
				return WRONG_DATE;
			}
		}
	}
}
EN_TERMINALERROR_T isCardExpired(ST_TERMINAL_DATA_T* terminalData, ST_CARD_T* card)
{
    printf("Checking Card Validty for Transacation..\n");
    // Extract month and year from cardExpirationDate (MM/YY)
    int cardMonth, cardYear;
    if (sscanf(card->cardExpirationDate, "%2d/%2d", &cardMonth, &cardYear) != 2) {
        printf("Invalid card expiration date format!\n");
        return INVALID_DATE;
    }
    // Extract year and month from transactionDate (YYYY/MM/DD)
    int transYear, transMonth;
    if (sscanf(terminalData->transactionDate, "%4d/%2d", &transYear, &transMonth) != 2) {

        return INVALID_DATE;
    }
    // Calculate the expiration year based on the card's expiration month
    int expYear = cardYear + 2000;
    if (transYear > expYear) {
        return EXPIRED_CARD;
    }
    if (transYear == expYear && transMonth > cardMonth) {
        return EXPIRED_CARD;
    }
    return OK_terminalError;
}
EN_TERMINALERROR_T getTransactionAmount(ST_TERMINAL_DATA_T* terminalData) {
    float amount = 0.0;
    int maxAttempts = 3;
    printf("Enter Transaction Amount: ");
    while (maxAttempts > 0) {
        int result = scanf("%f", &amount);
        if (result == 1) {
            if (amount < 0) {
                maxAttempts--;
                printf("Negative Values or Zero are not allowed!\n");
                if (maxAttempts == 0) {
                    printf("Attempts limit Excedded,Application will close!");
                    return INVALID_MAX_AMOUNT;
                }
            }
            else if (amount > 0) {
                terminalData->transAmount = amount;
                return OK_terminalError;
            }
            else {
                printf("Please Enter Correct Amount!\n");
            }
        }

    }
}
EN_TERMINALERROR_T setMaxAmount(ST_TERMINAL_DATA_T* terminalData) {
    int maxAttempts = 3;

    while (maxAttempts > 0) {
        float amount = 0.0;
        printf("Enter Max Transaction Amount: ");

        if (scanf("%f", &amount) == 1) {
            if (amount > 0) {
                terminalData->maxTransAmount = amount;
                return OK_terminalError;
            }
            else {
                printf("Invalid Amount! Please enter a positive value.\n");
            }
        }
        else {
            printf("Invalid Input! Please enter a valid numeric value.\n");
            maxAttempts--;

            if (maxAttempts == 0) {
                return INVALID_MAX_AMOUNT;
            }
        }
    }
    return INVALID_MAX_AMOUNT;
}
EN_TERMINALERROR_T isBelowMaxAmount(ST_TERMINAL_DATA_T* terminalData)
{
    if(terminalData->transAmount > terminalData->maxTransAmount){
        return EXCEED_MAX_AMOUNT;
    }
    return OK_terminalError;
}
#endif