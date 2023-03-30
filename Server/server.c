/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
#include "../Common/STD_types.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"


/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/
EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    /* ToDo*/
}


EN_serverError_t isValidAccount( ST_cardData_t *cardData,  ST_accountsDB_t *accountReference)
{
    /* ToDo*/
}


EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountReference)
{
    /* ToDo*/
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


EN_serverError_t saveTransaction( ST_transaction_t *transData )
{
    /* ToDo*/
}


void listSavedTransactions(void)
{
    /* ToDo*/
}

