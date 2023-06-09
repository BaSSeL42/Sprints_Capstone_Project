/**************************************************************************************************************************
 *                                                      File Guard
**************************************************************************************************************************/
#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_




/**************************************************************************************************************************
 *                                              User Defined Data Type
**************************************************************************************************************************/
typedef enum EN_transState_t
{
	APPROVED = 0,
	DECLINED_INSUFFECIENT_FUND,
	DECILINED_STOLEN_CARD,
	FRAUD_CARD,
    INTERNAL_SERVER_ERROR
	
}EN_transState_t;


typedef struct ST_transaction_t
{
	ST_cardData_t       cardHolderData;
	ST_terminalData_t   terminalData;
	EN_transState_t     transState;
    uint32_t            transactionSequenceNumber;
	
}ST_transaction_t; 


typedef enum EN_serverError_t
{
	SERVER_OK = 0,
	SAVING_FAILED,
	TRANSACTION_NOT_FOUND,
	ACCOUNT_NOT_FOUND,
    LOW_BALANCE,
    BLOCKED_ACCOUNT
	
}EN_serverError_t;

typedef enum EN_accountState_t
{
	RUNNING=0,
    BLOCKED
	
}EN_accountState_t;


typedef struct ST_accountsDB_t
{
	float balance;
    EN_accountState_t state;
    uint8_t primaryAccountNumber[20];
	
}ST_accountsDB_t; 

#define ACCOUNTS_DB_FIRST_INDEX		0
#define ACCOUNTS_DB_MAX_SIZE		255

#define TRANSACTION_DB_FIRST_INDEX	0
#define TRANSACTION_DB_MAX_SIZE		255

/**************************************************************************************************************************
 *                                               Function Prototype
**************************************************************************************************************************/
EN_transState_t recieveTransactionData(ST_transaction_t *transData);
EN_serverError_t isValidAccount( ST_cardData_t *cardData,  ST_accountsDB_t *accountReference);
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountReference);
EN_serverError_t isAmountAvailable( ST_terminalData_t *termData, ST_accountsDB_t *accountReference );
EN_serverError_t saveTransaction( ST_transaction_t *transData );

void listSavedTransactions(void);

EN_serverError_t savedb();
EN_serverError_t loaddb();
EN_serverError_t savedbAccounts();
EN_serverError_t loaddbAccounts();



/**************************************************************************************************************************
 *                                                  Function Prototype For Testing
**************************************************************************************************************************/

void recieveTransactionDataTest(void);
void isBlockedAccountTest(void);
void isAmountAvailableTest(void);
void isValidAccountTest(void);
void saveTransactionTest(void);

#endif /*SERVER_SERVER_H_*/