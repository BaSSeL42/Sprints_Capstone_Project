/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/


#include <stdio.h>
#include <string.h> 
#include "../Common/STD_types.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"

/**************************************************************************************************************************
 *                                              Global Variables
**************************************************************************************************************************/
ST_accountsDB_t accountsDB[ACCOUNTS_DB_MAX_SIZE] = 
{
    {2000.000,RUNNING,"4342077277183288"},
    {5000.000,BLOCKED,"51102000115511112"},
    {6600.000,RUNNING,"12345678910111213"},
    {2550.200,BLOCKED,"12111098765432100"}
};


ST_transaction_t transactionDB[TRANSACTION_DB_MAX_SIZE] =
{
    {
        {"sherif ashraf","51102000115511100","05/25"},
        {500,1000,"01-10-2022"},
        APPROVED,
        1
    },
    {
        {"yuossef","51102777668811100","04/24"},
        {1700,20000,"05-05-2021"},
        INTERNAL_SERVER_ERROR,
        2
    },
    {
        {"laila","51102888668811188","03/23"},
        {50000,60000,"06-06-2019"},
        FRAUD_CARD,
        3
    },
}; 
/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {

    EN_serverError_t returnedValue;
    ST_accountsDB_t* accountRef=NULL;
    uint8_t i;
    for (i = 0; i < 255; i++) {
        if (!strcmp(transData->cardHolderData.primaryAccountNumber, accountsDB[i].primaryAccountNumber))
        {
            accountRef = &accountsDB[i];
            break;
        }

    }
    returnedValue = isValidAccount(&transData->cardHolderData, accountRef);
    if (returnedValue == ACCOUNT_NOT_FOUND)return FRAUD_CARD;
    returnedValue = isAmountAvailable(&transData->terminalData, accountRef);
    if (returnedValue == LOW_BALANCE)return DECLINED_INSUFFECIENT_FUND;
    returnedValue = isBlockedAccount(accountRef);
    if (returnedValue == BLOCKED_ACCOUNT)return DECILINED_STOLEN_CARD;
    returnedValue = saveTransaction(transData);
    if (returnedValue == SAVING_FAILED)return INTERNAL_SERVER_ERROR;
    accountRef->balance -= transData->terminalData.transAmount;

    return SERVER_OK;
}


EN_serverError_t isValidAccount( ST_cardData_t *cardData,  ST_accountsDB_t *accountReference)
{
    EN_serverError_t LOCAL_returnValue = ACCOUNT_NOT_FOUND;
    uint32_t LOCAL_dataBaseLoopCounter;
    for(LOCAL_dataBaseLoopCounter = ACCOUNTS_DB_FIRST_INDEX ; LOCAL_dataBaseLoopCounter <= ACCOUNTS_DB_MAX_SIZE ; LOCAL_dataBaseLoopCounter++)
    {
        if(!strcmp((cardData->primaryAccountNumber),(accountsDB[LOCAL_dataBaseLoopCounter].primaryAccountNumber)))
        {
            *accountReference = accountsDB[LOCAL_dataBaseLoopCounter];
            LOCAL_returnValue = SERVER_OK;
            break;
        }
    }
    if(LOCAL_returnValue == ACCOUNT_NOT_FOUND)
    {
        accountReference = NULL;
    }
    return LOCAL_returnValue;
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

        //listSavedTransactions();
        return SERVER_OK;
    }

}


void listSavedTransactions(void)
{
    uint32_t LOCAL_dataBaseLoopCounter; 
    for(LOCAL_dataBaseLoopCounter = TRANSACTION_DB_FIRST_INDEX;LOCAL_dataBaseLoopCounter <= TRANSACTION_DB_MAX_SIZE ; LOCAL_dataBaseLoopCounter++)
    {
        if(transactionDB[LOCAL_dataBaseLoopCounter].transactionSequenceNumber != 0)
        {
            printf("######################################### \n\n");
            printf("Transaction Sequence Number : %d \n",transactionDB[LOCAL_dataBaseLoopCounter].transactionSequenceNumber);
            printf("Transaction Date            : %s \n",transactionDB[LOCAL_dataBaseLoopCounter].terminalData.transactionDate);
            printf("Transaction Amount          : %d \n",transactionDB[LOCAL_dataBaseLoopCounter].terminalData.transAmount);
            printf("Transaction State           : %d \n",transactionDB[LOCAL_dataBaseLoopCounter].transState);
            printf("Terminal Max Amount         : %d \n",transactionDB[LOCAL_dataBaseLoopCounter].terminalData.maxTransAmount);
            printf("Cardholder Name             : %s \n",transactionDB[LOCAL_dataBaseLoopCounter].cardHolderData.cardHolderName);
            printf("PAN                         : %s \n",transactionDB[LOCAL_dataBaseLoopCounter].cardHolderData.primaryAccountNumber);
            printf("Card Expiration Date        : %s \n",transactionDB[LOCAL_dataBaseLoopCounter].cardHolderData.cardExpirationDate);
            printf("######################################### \n\n");
        }
        else
        {
            break;
        }
    }
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


void isValidAccountTest(void)
{
    ST_cardData_t testCase1 = {"sherif ashraf","51102000115511100","05/25"};
    EN_serverError_t LOCAL_returnValue;
    ST_accountsDB_t LOCAL_returnAccountReffrence;
    printf("Tester Name : Sherif Ashraf \n");
    printf("Test Case 1 : \n");
    printf("Input Data : {sherif ashraf,51102000115511100,05/25} \n");
    printf("Expected Result : \nPrimary Account Number : 51102000115511100 \nState : 0 \nBalance : 2000.00 \n \n");
    LOCAL_returnValue = isValidAccount(&testCase1,&LOCAL_returnAccountReffrence);
    printf("Actual Result : %d\n",LOCAL_returnValue);
    printf("Primary Account Number : %s \n",LOCAL_returnAccountReffrence.primaryAccountNumber);
    printf("State : %d \n",LOCAL_returnAccountReffrence.state);
    printf("Balance : %.2f \n",LOCAL_returnAccountReffrence.balance);
}

