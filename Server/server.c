/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
#include "../Common/STD_types.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"


/**************************************************************************************************************************
 *                                              Global Variables
**************************************************************************************************************************/
ST_accountsDB_t  accountsDB[255];
ST_transaction_t transactionDB[255];
/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/
EN_transState_t recieveTransactionData(ST_transaction_t* transData) {

    EN_serverError_t returnedValue;
    ST_accountsDB_t *accountRef;
    uint8_t i;
    for (i = 0; i < 255; i++) {
        if (!strcmp(&transData->cardHolderData.primaryAccountNumber, &accountsDB[i].primaryAccountNumber))
        {
            accountRef = &accountDB[i];
            break;
        }

    }
    returnedValue = isValidAccount(&transData->cardHolderData, accountRef);
    if (returnedValue == ACCOUNT_NOT_FOUND)return FRAUD_CARD;
    returnedValue = isAmountAvailable(&transData->terminalData, accountRef);
    if (returnedValue == LOW_BALANCE)return DECLINED_INSUFFECIENT_FUND;
    returnedValue = isBlockedAccount(accountRef);
    if (returnedValue == BLOCKED_ACCOUNT)return DECILINED_STOLEN_CARD;
    returnedValue = saveTransaction(transData->transState);
    if (returnedValue == SAVING_FAILED)return INTERNAL_SERVER_ERROR;

    *accountRef.balance -= transData->terminalData.transAmount

    return SERVER_OK;


}


EN_serverError_t isValidAccount( ST_cardData_t *cardData,  ST_accountsDB_t *accountReference)
{
    /* ToDo*/
}


EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountReference)
{
    EN_serverError_t errorStatus = SERVER_OK;

    if( accountReference != NULL )
    {
        if (accountReference->state == BLOCKED)
        {
            errorStatus = BLOCKED_ACCOUNT;
        }
        else if (accountReference->state == RUNNING)
        {
            errorStatus = SERVER_OK;
        }
        else 
        {
            /*do nothing*/
        }
    }
    else 
    {
        errorStatus = ACCOUNT_NOT_FOUND;
    }


    return errorStatus;
}



EN_serverError_t isAmountAvailable( ST_terminalData_t *termData, ST_accountsDB_t *accountReference )
{
	/* Check that transaction value is less than account balance */
	if (termData->transAmount > accountReference->balance)
	{
		return LOW_BALANCE;
	}
	
	return SERVER_OK;
}


EN_serverError_t saveTransaction(ST_transaction_t* transData) {
    uint8_t i;
    for (i = 0; i < 255; i++) {
        if (transactionDB[i].transactionSequenceNumber == 0) {
            if (i == 0)transData->transactionSequenceNumber = 1;
            else transData->transactionSequenceNumber = transactionDB[i - 1].transactionSequenceNumber + 1;
            transactionDB[i] = *transData;
            break;
        }

        listSavedTransactions();
        return SERVER_OK;
    }

}


void listSavedTransactions(void)
{
    /* ToDo*/
}





/**************************************************************************************************************************
 *                                                  Function Implementation For Testing
**************************************************************************************************************************/
int32_t ret;
ST_accountsDB_t accountDB;
void isBlockedAccountTest(void)
{
    printf("Test Case1 \n");
    printf("Input: BLOCKED\n");

    printf(" expected output : BLOCKED_ACCOUNT\n");
    accountDB.state = BLOCKED;

    ret = isBlockedAccount( &accountDB);

    if (ret == SERVER_OK)
    {
        printf("Actual result: SERVER_OK \n");
    }
    else if( ret == BLOCKED_ACCOUNT )
    {
        printf("Actual result: BLOCKED_ACCOUNT\n");
    }
    printf("..........................................................................\n\n");

    printf("Test Case2 \n");
    printf("Input: state: RUNNING\n");

    printf(" expected output : SERVER_OK\n");
    accountDB.state = RUNNING;
    ret = isBlockedAccount( &accountDB);

    if (ret == SERVER_OK)
    {
        printf("Actual result: SERVER_OK \n");
    }
    else if( ret == BLOCKED_ACCOUNT )
    {
        printf("Actual result: BLOCKED_ACCOUNT\n");
    }

}


/*
Tester Name: Alaa Hisham

Function Name: isAmountAvailable
*/

void isAmountAvailableTest(void)
{
	ST_terminalData_t termData = { 0,0,{0} };
	ST_accountsDB_t account = { 5000,0,{0} };
	uint8_t expected, result;

	printf("Testing (isValidCardPAN) function\n\n");

	/* Test Case 1: transaction amount more than account balance */
	termData.transAmount = 5000.1;
	expected = LOW_BALANCE;

	result = isAmountAvailable(&termData, &account);

	if (result == LOW_BALANCE)
	{
		printf("Test case 1 passed!\n\n");
	}
	else
	{
		printf("Test case 1 failed!\n\n");
	}

	/* Test Case 2: transaction amount same as account balance */
	termData.transAmount = 5000;
	expected = SERVER_OK;

	result = isAmountAvailable(&termData, &account);

	if (result == SERVER_OK)
	{
		printf("Test case 2 passed!\n\n");
	}
	else
	{
		printf("Test case 2 failed!\n\n");
	}

	/* Test Case 3: transaction amount less than account balance  */
	termData.transAmount = 4999.9;
	expected = SERVER_OK;

	result = isAmountAvailable(&termData, &account);

	if (result == SERVER_OK)
	{
		printf("Test case 3 passed!\n\n");
	}
	else
	{
		printf("Test case 3 failed!\n\n");
	}
}
