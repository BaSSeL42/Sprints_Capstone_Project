/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
#include "../Common/STD_types.h"
#include "../Card/card.h"
#include "terminal.h"


/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/
EN_terminalError_t getTransactionData( ST_terminalData_t *termData )
{
    /* ToDo */
}


EN_terminalError_t isCardExpired( ST_cardData_t *cardData, ST_terminalData_t *termData )
{
	/* Compare year and month digits from most significant to least */
	if (cardData->cardExpirationDate[EXP_DATE_Y_TENS] < termData->transactionDate[TRANS_DATE_Y_TENS])
	{
		return EXPIRED_CARD;
	}
	else if (cardData->cardExpirationDate[EXP_DATE_Y_TENS] == termData->transactionDate[TRANS_DATE_Y_TENS])
	{
		if (cardData->cardExpirationDate[EXP_DATE_Y_UNITS] < termData->transactionDate[TRANS_DATE_Y_UNITS])
		{
			return EXPIRED_CARD;
		}
		else if (cardData->cardExpirationDate[EXP_DATE_Y_TENS] == termData->transactionDate[TRANS_DATE_Y_TENS])
		{
			if (cardData->cardExpirationDate[EXP_DATE_M_TENS] < termData->transactionDate[TRANS_DATE_M_TENS])
			{
				return EXPIRED_CARD;
			}
			else if (cardData->cardExpirationDate[EXP_DATE_M_TENS] == termData->transactionDate[TRANS_DATE_M_TENS])
			{
				if (cardData->cardExpirationDate[EXP_DATE_M_UNITS] < termData->transactionDate[TRANS_DATE_M_UNITS])
				{
					return EXPIRED_CARD;
				}
			}
		}

	}

	return TERMINAL_OK;
}


EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData )
{
    /* ToDo */
}


EN_terminalError_t isBelowMaxAccount(ST_terminalData_t *termData )
{
    /* ToDo */
}


EN_terminalError_t setMaxAccount(ST_terminalData_t *termData , float maxAccount)
{
    /* ToDo */
}


EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
	int8_t LocalIterator, PAN_sum = 0, char2num;
	uint8_t PAN_length = strlen(cardData->primaryAccountNumber);
	uint8_t second = 0;

	for (LocalIterator = PAN_length-1; LocalIterator >= 0; LocalIterator--)
	{
		char2num = cardData->primaryAccountNumber[LocalIterator] - '0';

		if (second)
		{	/* Double every other digit in PAN */
			char2num *= 2;
			/* if result is >9, add the digits */
			if (char2num > 9)
			{
				char2num = char2num/10 + char2num%10;
			}
		}
		
		PAN_sum += char2num;
		/* Toggle */
		second ^= 1;
	}

	if (PAN_sum % 10 != 0) return INVALID_CARD;

	return TERMINAL_OK;
}