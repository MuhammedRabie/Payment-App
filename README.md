# Payment application
Payment systems are now available everywhere and everyone interacts with these systems every day.
There are different types of transactions you can make, SALE, REFUND, Pre-Authorization, and VOID.

- SALE: means to buy something and its price will be deducted from your bank account.
- REFUND: this means that you will return something and wants your money back to your bank account.
- Pre-Authorization: means holding an amount of money from your account, e.g Hotel reservation.
- VOID: this means canceling the transaction, e.g if the seller entered the wrong amount.
  
I Implemented the SALE transaction only by simulating the card, terminal(ATM), and the server.
# Project flow chart 
![183305187-4d1241fb-fa97-4daf-8a6b-a1f41a540ac7](https://github.com/MuhammedRabie/Payment-App/assets/66913233/3b3bfcd2-179b-4f67-9cdd-75f5b58184a5)
# Functions
only a part not all : 
- appStart(): called in Source.c to run the application.
- getTransactionDate(): asks user to enter Date with format [YYYY/MM/DD] or retreives current system date.
- saveTransaction():Saves APPROVED transactions in transactions.txt file.
- updateBalance(): Updates Balance after each transaction and stores new balance in accounts.txt .
# Test Cases
1. Transaction approved user story.
As a bank customer have an account and has a valid and not expired card, I want to withdraw an amount of money less than the maximum allowed and less than or equal to the amount in my balance, so that I am expecting that the transaction is approved and my account balance is reduced by the withdrawn amount.

2. Exceed the maximum amount user story:
As a bank customer have an account, that has a valid and not expired card, I want to withdraw an amount of money that exceeds the maximum allowed amount so that I am expecting that the transaction declined.

3. Insufficient fund user story:
As a bank customer have an account and has a valid and not expired card, I want to withdraw an amount of money less than the maximum allowed and larger than the amount in my balance so that I am expecting that the transaction declined.

4. Expired card user story:
As a bank customer have an account, have a valid but expired card, I want to withdraw an amount of money so that I expect that the transaction declined.

5. Invalid card user story:
As a bank customer have an account and has a valid and not expired stolen card, I want to block anyone from using my card so that I am expecting that any transaction made by this card is declined.
