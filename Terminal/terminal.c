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
    int32_t x, flag = 0;

    printf("Please Enter Transaction Date: ");
    scanf("%s", transDate);

    uint8_t* token = strtok(transDate, "/");
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


EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData )
{
    /* ToDo */
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
