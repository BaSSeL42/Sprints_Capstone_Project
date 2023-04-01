/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
#include "app.h"
//#include "../Server/server.h"

/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/
extern ST_accountsDB_t accountsDB[ACCOUNTS_DB_MAX_SIZE];

void appStart(void)
{


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
            printf("Wrong Name!\n");
            break;
        }
        
        Return_cardError = getCardExpiryDate(&client_data);
        
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
            printf("Invalid date!\n");
            break;
        }

        Return_terminalError = isCardExpired(&client_data,&client_transactions);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */
        }
        else
        {
            printf("Transaction Failed! Card Expired!\n");
            break;
        }

        Return_terminalError = isValidCardPAN(&client_data);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */ 
        }
        else
        {
            printf("Invalid PAN! Invalid Card!\n");
            break;
        }

        Return_terminalError = getTransactionAmount(&client_transactions);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */
        }
        else
        {
            printf("Invalid amount!\n");
            break;
        }

        Return_terminalError = isBelowMaxAmount(&client_transactions);

        if(Return_terminalError == TERMINAL_OK)
        {
            /* Do Nothing */
        }
        else
        {
            printf("Maximum transaction amount exceeded!\n");
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
            //listSavedTransactions();
        }
        else
        {
            break;
        }
        break;
    }

    
   /* getCardHolderNameTest();
    getCardExpiryDateTest();*/
    /*getCardPanTest();*/
   /* getTransactionDateTest();
    isCardExpiredTest();
    setMaxAmountTest();
    isValidCardPANTest();
    isBelowMaxAccountTest();
    getTransactionAmountTest();
    recieveTransactionDataTest();
    isBlockedAccountTest();
    isAmountAvailableTest();
    isValidAccountTest();*/
   // saveTransactionTest();


}