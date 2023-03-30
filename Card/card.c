/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
#include "../Common/STD_types.h"
#include "card.h"



/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint8_t size, Local_Iterator=0;

	printf("Card Holder Name: ");
	scanf("%[^\n]%*c", cardData->cardHolderName);
	printf("%s\n", cardData->cardHolderName);
	
	/* Check whether the name is within limits */
	size = strlen(cardData->cardHolderName);
	if (size < 20 || size > 24)
	{
		return WRONG_NAME;
	}

	/* Check that all characters are alphabetic */
	while (cardData->cardHolderName[Local_Iterator] != '\0')
	{
		if ((cardData->cardHolderName[Local_Iterator] < A_ASCII_INDEX || cardData->cardHolderName[Local_Iterator] > Z_ASCII_INDEX)
			&& (cardData->cardHolderName[Local_Iterator] < a_ASCII_INDEX || cardData->cardHolderName[Local_Iterator] > z_ASCII_INDEX)
			&& cardData->cardHolderName[Local_Iterator] != SPACE_ASCII_INDEX)
		{
			return WRONG_NAME;
		}

		Local_Iterator++;
	}

	return CARD_OK;
}


EN_cardError_t getCardExpirayDate(ST_cardData_t* cardData)
{
	/* ToDo */
}


EN_cardError_t getCardPan(ST_cardData_t* cardData)
{
	
	/* ToDo */
}
