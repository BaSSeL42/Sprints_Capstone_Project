/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Common/STD_types.h"
#include "../Card/card.h"
#include "terminal.h"


/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/
EN_terminalError_t getTransactionDate( ST_terminalData_t *termData )
{
    EN_terminalError_t errorStatus;
    uint8_t transDate[50];
    uint8_t buffer[50];
    int32_t x, flag = 0;

    printf("Please Enter Transaction Date: ");
    scanf("%s", transDate);
    strcpy( buffer, transDate );

    uint8_t* token = strtok(buffer, "/");
    while(token != NULL)
    {
        x = atoi(token);
        if ( flag == 0  )
        {
            if ( x > 0 && x < 31 )
            {
                flag = 1;
            }
            else 
            {
                break;
            }
        }
        else if (flag == 1)
        {
            if ( x >= 1 && x <= 12 )
            {
                flag = 2;
            }
            else 
            {
                break;
            }
        }
        else if ( flag == 2 )
        {
            if ( x > 2000 && x < 2100 )
            {
                flag = 3;
            }
            else 
            {
                break;
            }
        }
        token = strtok(NULL, "/");
    }

    
    
    if(termData == NULL || strlen(transDate) < 10 || flag != 3)
    {
        errorStatus = WRONG_DATE;
    }
    else 
    {
        errorStatus = TERMINAL_OK;
        strcpy( termData->transactionDate, transDate );
    }


    return errorStatus;
}


EN_terminalError_t isCardExpired( ST_cardData_t *cardData, ST_terminalData_t *termData )
{
    /* ToDo */
}


EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
    float amount;
    printf("ENTER THE TRANSACTION AMOUNT : ");
    scanf("%f", &amount);
    if (amount <= 0)return INVALID_AMOUNT;
    termData->transAmount = amount;
    return TERMINAL_OK;


}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData )
{
    /* ToDo */
}


EN_terminalError_t setMaxAmount(ST_terminalData_t *termData , float maxAmount)
{
    EN_terminalError_t errorStatus;
    if (termData != NULL)
    {
        if ( maxAmount > 0 )
        {
            errorStatus = TERMINAL_OK;
            termData->maxTransAmount = maxAmount;
        }
        else if ( maxAmount <= 0 )
        {
            errorStatus = INVALID_MAX_AMOUNT;
        }
        else 
        {
            /*do nothing*/
        }
    }
    else 
    {
        /* do nothing */
    }

    return errorStatus;
}


EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    /* ToDo */
}




/**************************************************************************************************************************
 *                                                  Function Implementation For Testing
**************************************************************************************************************************/
ST_terminalData_t terminalData;
int32_t ret;

void getTransactionDateTest(void)
{
    printf("Test Case1 \n");
    printf("Input: \"25/03/2010\" \n");

    printf(" expected output : Ok\n");

    ret = getTransactionDate( &terminalData );
    if (ret == TERMINAL_OK)
    {
        printf("Actual result: OK \n");
    }
    else if( ret == WRONG_DATE )
    {
        printf("WRONG_DATE\n");
    }

    printf("Test Case2 \n");
    printf("Input: \"255/3/2010\" \n");

    printf(" expected output : wrong Date\n");

    ret = getTransactionDate( &terminalData );
    if (ret == TERMINAL_OK)
    {
        printf("Actual result: OK \n");
    }
    else if( ret == WRONG_DATE )
    {
        printf("WRONG_DATE\n");
    }
}



void setMaxAmountTest (void)
{
    printf("Test Case1 \n");
    printf("Input: 10000.0 \n");

    printf(" expected output : Ok\n");

    ret = setMaxAmount( &terminalData,  10000.0 );

    if (ret == TERMINAL_OK)
    {
        printf("Actual result: OK \n");
    }
    else if( ret == INVALID_MAX_AMOUNT )
    {
        printf("Actual result: INVALID_MAX_AMOUNT\n");
    }
    printf("..........................................................................\n\n");
    printf("Test Case2 \n");
    printf("Input: -1200 \n");

    printf(" expected output : Invalid Max Amount\n");

    ret = setMaxAmount( &terminalData,  -1200 );

    if (ret == TERMINAL_OK)
    {
        printf("Actual result: OK \n");
    }
    else if( ret == INVALID_MAX_AMOUNT )
    {
        printf("Actual result: INVALID_MAX_AMOUNT\n");
    }

}


void getTransactionAmountTest(void) {
    ST_terminalData_t x;
    EN_terminalError_t returnedval;
    returnedval = getTransactionAmount(&x);
    printf("Tester Name : Sharpel Malak\n");
    printf("Testcase 4  : user enters positive and right amount \n");
    printf("Input data  :  5000\n");
    printf("Expected result : 0\n");
    printf("Actual result : %d\n", returnedval);
}
