/**************************************************************************************************************************
 *                                                      File Guard
**************************************************************************************************************************/
#ifndef CARD_CARD_H_
#define CARD_CARD_H_

/**************************************************************************************************************************
 *                                              User Defined Data Type
**************************************************************************************************************************/
typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
	
}ST_cardData_t; 


typedef enum EN_cardError_t
{
	CARD_OK = 0,
	WRONG_NAME,
	WRONG_EXP_DATE,
	WRONG_PAN
	
}EN_cardError_t;


/**************************************************************************************************************************
 *                                                  Function Prototype
**************************************************************************************************************************/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpirayDate(ST_cardData_t* cardData);
EN_cardError_t getCardPan(ST_cardData_t* cardData);




#endif /*CARD_CARD_H_*/