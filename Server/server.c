/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/


#include <stdio.h>
#include <string.h> 
#include "../Common/STD_types.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"

#pragma warning(disable : 4996)

/**************************************************************************************************************************
 *       {10000.000,RUNNING,"4342077277183288"},
    {5000.000,RUNNING,"51102000115511112"},
    {6600.000,RUNNING,"12345678910111213"},
    {2550.200,BLOCKED,"12111098765432100"}     
 
 
 
 
 Global Variables
**************************************************************************************************************************/
ST_accountsDB_t accountsDB[ACCOUNTS_DB_MAX_SIZE];

ST_transaction_t transactionDB[TRANSACTION_DB_MAX_SIZE];
/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {


    loaddb();
    loaddbAccounts();
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
    if (returnedValue == ACCOUNT_NOT_FOUND) 
    {
        transData->transState = FRAUD_CARD;
        returnedValue = saveTransaction(transData);
        return FRAUD_CARD;
    } 

    returnedValue = isAmountAvailable(&transData->terminalData, accountRef);
    if (returnedValue == LOW_BALANCE)
    {
        transData->transState = DECLINED_INSUFFECIENT_FUND;
        returnedValue = saveTransaction(transData);
        return DECLINED_INSUFFECIENT_FUND;
    }

    returnedValue = isBlockedAccount(accountRef);
    if (returnedValue == BLOCKED_ACCOUNT)
    {
        transData->transState = DECILINED_STOLEN_CARD;
        returnedValue = saveTransaction(transData);
        return DECILINED_STOLEN_CARD;
    }

    if (returnedValue == SAVING_FAILED)
    {
        transData->transState = INTERNAL_SERVER_ERROR;
        returnedValue = saveTransaction(transData);
        return INTERNAL_SERVER_ERROR;
    }
    transData->transState = APPROVED;
    accountRef->balance -= transData->terminalData.transAmount;
    savedbAccounts();
    //printf("\n--- new balance : %f\n", accountRef->balance);
    returnedValue = saveTransaction(transData);

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
           
            transData->transactionSequenceNumber = i + 1;
            transactionDB[i].transactionSequenceNumber = transData->transactionSequenceNumber;
            transactionDB[i].transState = transData->transState;
            strcpy(transactionDB[i].cardHolderData.cardHolderName, transData->cardHolderData.cardHolderName);
            strcpy(transactionDB[i].cardHolderData.cardExpirationDate, transData->cardHolderData.cardExpirationDate);
            strcpy(transactionDB[i].cardHolderData.primaryAccountNumber,transData->cardHolderData.primaryAccountNumber);
            transactionDB[i].terminalData.maxTransAmount = transData->terminalData.maxTransAmount;
            transactionDB[i].terminalData.transAmount = transData->terminalData.transAmount;
            strcpy(transactionDB[i].terminalData.transactionDate, transData->terminalData.transactionDate);
            break;
        }

       
    }
    savedb();
    listSavedTransactions();
    return SERVER_OK;

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
            printf("Transaction Amount          : %f \n",transactionDB[LOCAL_dataBaseLoopCounter].terminalData.transAmount);
            printf("Transaction State           : %d \n",transactionDB[LOCAL_dataBaseLoopCounter].transState);
            printf("Terminal Max Amount         : %f \n",transactionDB[LOCAL_dataBaseLoopCounter].terminalData.maxTransAmount);
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

EN_serverError_t savedb() {
    FILE* fptr;
    int i;
    if ((fptr = fopen("transdatabase.bin", "wb")) == NULL) {
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        return INTERNAL_SERVER_ERROR;
    }

    for(i=1;i<255;++i)

    fwrite(transactionDB, sizeof(ST_transaction_t),255, fptr);

    fclose(fptr);

    return SERVER_OK;


}
EN_serverError_t loaddb() {
    FILE* fptr;
    uint8_t i;
    ST_transaction_t transData;
    if ((fptr = fopen("transdatabase.bin", "rb")) == NULL) {
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        return INTERNAL_SERVER_ERROR;
    }
    for(i=0;i<255;++i){
     fread(transactionDB, sizeof(struct ST_transaction_t), 255, fptr);
    // printf("%d", transactionDB->transState);

    }
   
   // listSavedTransactions();
    fclose(fptr);

}

EN_serverError_t savedbAccounts() {
    FILE* fptr;
    uint8_t i;
    if ((fptr = fopen("accountsDB.txt", "w")) == NULL) {
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        return INTERNAL_SERVER_ERROR;
    }

    for (i = 0; i < 255; ++i)
    {
        /* break when we save last stored value in the array */
        if (accountsDB[i].balance == 0) break;
        /* print accounts' data line by line in the database */
        fprintf(fptr, "%f,%d,%s\n", accountsDB[i].balance, accountsDB[i].state, accountsDB[i].primaryAccountNumber);
    }

    fclose(fptr);

    return SERVER_OK;
}

EN_serverError_t loaddbAccounts() {
    FILE* fptr;
    uint8_t i=0;
    char buffer[100];
    ST_transaction_t transData;

    if ((fptr = fopen("accountsDB.txt", "r")) == NULL) {
        printf("Error! opening file");

        return INTERNAL_SERVER_ERROR;
    }
    /* get first line from file */
    fgets(buffer, 100, fptr);
    while (!feof(fptr)) {
        /* scan the buffer into the elements of each accountsDB struct */
        sscanf(buffer, "%f,%d,%[^\n]", &accountsDB[i].balance, &accountsDB[i].state, accountsDB[i].primaryAccountNumber);
        fgets(buffer, 100, fptr);
        i++;
    }

    fclose(fptr);

    return SERVER_OK;
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


void recieveTransactionDataTest(void) {
    
    ST_transaction_t testCases1 = { {"sherif ashraf mostafa","4342077277183288","05/25"},{ 1000,MAX_TRANS_AMOUNT,"30/03/2023"}, };
    ST_transaction_t testCases2 = { {"sherif ashraf mostafa","51102000115511112","05/25"},{1000,MAX_TRANS_AMOUNT,"30/03/2023"}, };
    ST_transaction_t testCases3 = { {"sherif ashraf mostafa","4342077277183288","05/25"},{ 2500,MAX_TRANS_AMOUNT,"30/03/2023"}, };
    EN_serverError_t LOCAL_returnValue;

    printf("Tester Name : Sharpel Malak \n");
    printf("Test Case 1 : Approved data\n");
    printf("Input Data : {sherif ashraf mostafa,4342077277183288,05/25} ,{ 1000,MAX_TRANS_AMOUNT,30/03/2023}\n");
    printf("Expected Result : 0 \n");
    LOCAL_returnValue = recieveTransactionData(&testCases1);
    printf("Actual Result : %d\n\n", LOCAL_returnValue);
///////////////////////////
    printf("Tester Name : Sharpel Malak \n");
    printf("Test Case 2 : Stolen Card\n");
    printf("Input Data : {sherif ashraf mostafa,51102000115511112,05/25} ,{ 1000,MAX_TRANS_AMOUNT,30/03/2023}\n");
    printf("Expected Result : 2 \n");
    LOCAL_returnValue = recieveTransactionData(&testCases2);
    printf("Actual Result : %d\n", LOCAL_returnValue);

    ///////////////////////////
    printf("Tester Name : Sharpel Malak \n");
    printf("Test Case 3 : Low balance\n");
    printf("Input Data : {sherif ashraf mostafa,4342077277183288,05/25} ,{ 2500,MAX_TRANS_AMOUNT,30/03/2023}\n");
    printf("Expected Result :1 \n");
    LOCAL_returnValue = recieveTransactionData(&testCases3);
    printf("Actual Result : %d\n", LOCAL_returnValue);


}
void saveTransactionTest(void) {
    printf("DATA BASE BEFORE SAVING TRANS\n");
    listSavedTransactions();
    ST_transaction_t testCases1 = { {"sherif ashraf mostafa","4342077277183288","05/25"},{ 1000,MAX_TRANS_AMOUNT,"30/03/2023"}, };
    EN_serverError_t LOCAL_returnValue;

    printf("Tester Name : Sharpel Malak \n");
    printf("Test Case 1 : Approved data\n");
    printf("Input Data : {sherif ashraf mostafa,4342077277183288,05/25} ,{ 1000,MAX_TRANS_AMOUNT,30/03/2023}\n");
    printf("Expected Result :  adding one transaction in DB\n");
    printf("Actual Result : \n\n");
    LOCAL_returnValue = saveTransaction(&testCases1);
    printf("Expected Return result :0 \n", LOCAL_returnValue);
    printf("Actual Return result : %d \n",LOCAL_returnValue);
   

}

/*

    {2000.000,RUNNING,"4342077277183288"},
    {5000.000,RUNNING,"51102000115511112"},
    {6600.000,RUNNING,"12345678910111213"},
    {2550.200,BLOCKED,"12111098765432100"}


*/