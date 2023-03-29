/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
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

    if(termData != NULL)
    {
        gets( termData->transactionDate );
    }
    else 
    {
        errorStatus = INVALID_CARD;
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
        errorStatus = TERMINAL_OK;
        termData->maxTransAmount = maxAmount;
    }
    else 
    {
        errorStatus = INVALID_CARD;
    }

    return errorStatus;
}


EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    /* ToDo */
}