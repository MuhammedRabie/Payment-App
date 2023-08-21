#ifndef CARD_C // header guard file.
#define CARD_C
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "card.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#include<time.h>
int getLuhnDigit(const char* PAN) {
    int sum = 0;
    char reversedPAN[16];
    char temp[16];
    for (int i = 0; i <16; i++) {
        temp[i] = PAN[i];
        reversedPAN[15 - i] = temp[i];
    }
    for (int i = 0; i < 16; i++) {
        int digit = reversedPAN[i]-'0';
        if (reversedPAN[i] % 2 == 0) {
            digit = digit * 2;
            if (digit > 9) {
                digit = digit - 9;
            }
        }
        sum = sum + digit;
    }
    int checkDigit = (10 - (sum % 10)) % 10;
    return checkDigit;
}
EN_CARD_ERROR_T getCardHolderName(const ST_CARD_T* card)
{
    int maxAttempts = 3;
    while (maxAttempts > 0) {
        printf("Enter Card Holder Name [20-24] Characters: ");
        uint8_t name[100];
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0'; // Remove the newline character from the end of the string
        if (strlen(name) >= 20 && strlen(name) <= 24) {
            strcpy(card->cardHolderName, name);
            printf("Name is Added to Card!\n%s\n", card->cardHolderName);
            return VALID;
        }
        else {
            printf("Invalid name length!\n");
            maxAttempts--;
            if (maxAttempts == 0) {
                printf("Max Attempts Reached!\n,Application will terminate.");
                return INVALID_NAME;
            }
        }
    }
}
EN_CARD_ERROR_T getCardExpiryDate(ST_CARD_T* card) {
    int maxAttempts = 3;
    printf("Enter Card EXPIRY DATE [MM/YY]: ");
    while (maxAttempts > 0) {
        if (maxAttempts < 3) {
            printf("Re-enter Expiry Date [MM/YY]: ");
        }
        char temp[10];
        if (fgets(temp, sizeof(temp), stdin) == NULL) {
            printf("Error reading input!\n");
            return INVALID_DATE;
        }

        temp[strcspn(temp, "\n")] = '\0'; // Remove the newline character if present

        if (strlen(temp) != 5 || temp[2] != '/') {
            printf("Invalid date format!\n");
            maxAttempts--;

            if (maxAttempts == 0) {
                printf("Max Attempts Reached! Application will terminate.\n");
                return INVALID_DATE;
            }

            continue;
        }

        int month, year;
        if (sscanf(temp, "%2d/%2d", &month, &year) != 2 || month < 1 || month > 12 || year < 0 || year > 99) {
            printf("Invalid Expiry Date!\n");
            maxAttempts--;

            if (maxAttempts == 0) {
                printf("Max Attempts Reached! Application will terminate.\n");
                return INVALID_DATE;
            }

            continue;
        }

        strcpy(card->cardExpirationDate, temp);
        printf("Expiry date is added to card: %s\n", card->cardExpirationDate);
        return VALID;
    }

    return INVALID_DATE;
}
EN_CARD_ERROR_T getCardPAN(ST_CARD_T* card) {
    int maxAttempts = 3;
    while (maxAttempts > 0) {
        printf("Please Enter the 16-digit PAN for the card (Attempt %d):\n", 4 - maxAttempts);
        char temp[30];
        if (fgets(temp, sizeof(temp), stdin) == NULL) {
            printf("Error!\n");
            maxAttempts--;
            continue;
        }
        temp[strcspn(temp, "\n")] = '\0';

        if (strlen(temp) != 16) {
            printf("Incorrect PAN Length\n");
            maxAttempts--;
            continue;
        }

        strcpy(card->PAN, temp);
        return VALID;
    }

    printf("Maximum attempts reached. Exiting...\n");
    return INVALID_PAN;
}

#endif