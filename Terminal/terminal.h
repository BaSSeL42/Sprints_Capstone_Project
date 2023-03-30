/**************************************************************************************************************************
 *                                                      File Guard
**************************************************************************************************************************/
#ifndef TERMINAL_TERMINAL_H_
#define TERMINAL_TERMINAL_H_


/**************************************************************************************************************************
 *                                              		  Macros
**************************************************************************************************************************/
#define TRANS_DATE_Y_UNITS    9
#define TRANS_DATE_Y_TENS     8
#define TRANS_DATE_M_UNITS    4    
#define TRANS_DATE_M_TENS     3


/**************************************************************************************************************************
 *                                              User Defined Data Type
**************************************************************************************************************************/
typedef struct ST_terminalData_t
{
	float transAmount;
    float maxTransAmount;
    uint8_t transactionDate[11];
	
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
EN_terminalError_t getTransactionDate( ST_terminalData_t *termData );
EN_terminalError_t isCardExpired( ST_cardData_t *cardData, ST_terminalData_t *termData );
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData );
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData );
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData , float maxAmount);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);




/**************************************************************************************************************************
 *                                                  Function Prototype For Testing
**************************************************************************************************************************/
void getTransactionDateTest(void);
void isCardExpiredTest(void);
void setMaxAmountTest (void);
void isValidCardPANTest(void);


#endif /*TERMINAL_TERMINAL_H_*/