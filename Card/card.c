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
	/* ToDo */
}


EN_cardError_t getCardExpirayDate(ST_cardData_t* cardData) {

	uint8_t str[MAX_SIZE] = {}, temp[MAX_SIZE] = {};
	printf("ENTER EXPIRE DATE IN THIS FORMAT MM/YY : ");
	fgets(&str, MAX_SIZE, stdin);
	if (strlen(str) != MAX_EXPIRE || str[2] != '/')return WRONG_EXP_DATE;
	strcpy(temp, str);
	const char s[2] = "/";
	char* token;

	token = strtok(str, s);
	int i = 0;
	while (token != NULL) {
		//	printf("\n\n%s\n\n",token);
		if (i == 0 && (atoi(token) > MAX_MONTH || atoi(token) < MIN_MONTH))return WRONG_EXP_DATE;
		token = strtok(NULL, s);
		i++;
	}
	//printf("%s----", cardData->cardExpirationDate);
	for (i = 0; i < 6; i++)cardData->cardExpirationDate[i] = temp[i];
	return CARD_OK;

}


EN_cardError_t getCardPan(ST_cardData_t* cardData)
{
	
	/* ToDo */
}



////////////////////////////////////////////////////


void getCardExpirayDateTest(void) {

	ST_cardData_t x;
	EN_cardError_t returnedval;
	returnedval = getCardExpirayDate(&x);
	printf("Tester Name : Sharpel Malak\n");
	printf("Testcase 5  : user enters wrong format \n");
	printf("Input data  :  15215\n");
	printf("Expected result : 2\n");
	printf("Actual result : %d\n", returnedval);

}
