/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
#include "app.h"

/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/


void appStart(void)
{

    loaddbAccounts();
    loaddb();
    listSavedTransactions();
	ST_cardData_t client_data;
    ST_terminalData_t client_transactions;


    ST_accountsDB_t accountReference;

    EN_cardError_t Return_cardError;

    EN_terminalError_t Return_terminalError;

    EN_transState_t Return_serverError;
   

    while(1)
    {
        
        Return_cardError = getCardHolderName(&client_data);
        if(Return_cardError == CARD_OK)
        {
            /* Do Nothing */
        }
        else
        {
            break;
        }
        
        Return_cardError = getCardExpirayDate(&client_data); 
        
        if(Return_cardError == CARD_OK)
        {
            /* Do Nothing */
        }
        else
        {
            break;
        }

        Return_cardError = getCardPan(&client_data);

        if(Return_cardError == CARD_OK)
        {
            /* Do Nothing */
        }
        else
        {
            break;
        }

        Return_terminalError = getTransactionDate(&client_transactions);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */
        }
        else
        {
            break;
        }

        Return_terminalError = isCardExpired(&client_data,&client_transactions);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */
        }
        else
        {
            break;
        }

        Return_terminalError = isValidCardPAN(&client_data);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */ 
        }
        else
        {
            break;
        }

        Return_terminalError = getTransactionAmount(&client_transactions);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */
        }
        else
        {
            break;
        }

        Return_terminalError = isBelowMaxAmount(&client_transactions);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */
        }
        else
        {
            break;
        }

        ST_transaction_t client_server = 
        {
             client_data,
            client_transactions,
        };
        Return_serverError = recieveTransactionData(&client_server);

        if(Return_serverError == SERVER_OK)
        {
            /* Do Nothing */
            listSavedTransactions();
        }
        else
        {
            break;
        }
        break;
    }
 

}