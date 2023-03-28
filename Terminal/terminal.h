/**************************************************************************************************************************
 *                                                      File Guard
**************************************************************************************************************************/
#ifndef TERMINAL_TERMINAL_H_
#define TERMINAL_TERMINAL_H_


/**************************************************************************************************************************
 *                                              User Defined Data Type
**************************************************************************************************************************/
typedef struct ST_terminalData_t
{
	float transAmount;
    float maxTransAmount;
    uint8_t transactionData[11];
	
}ST_terminalData_t; 


typedef enum EN_terminalError_t
{
	TERMINAL_OK = 0,
    WRONG_DATE,
    EXPIRED_CARD,
    INVALID_CARD,
    INVALID_AMOUNT,
    EXCEED_MAX_ACCOUNT,
    INVALID_MAX_AMOUNT
	
}EN_terminalError_t;





/**************************************************************************************************************************
 *                                                  Function Prototype
**************************************************************************************************************************/
EN_terminalError_t getTransactionData( ST_terminalData_t *termData );
EN_terminalError_t isCardExpired( ST_cardData_t *cardData, ST_terminalData_t *termData );
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData );
EN_terminalError_t isBelowMaxAccount(ST_terminalData_t *termData );
EN_terminalError_t setMaxAccount(ST_terminalData_t *termData , float maxAccount);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);


#endif /*TERMINAL_TERMINAL_H_*/