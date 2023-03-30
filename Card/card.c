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



/**************************************************************************************************************************
 *                                                  Function Implementation For Testing
**************************************************************************************************************************/

/*
Tester Name: Alaa Hisham

Function Name: getCardHolderName
*/

void getCardHolderNameTest(void)
{
	uint8_t expected, result;
	ST_cardData_t testCard = { {0},{0},{0} };

	printf("Testing (getCardHolderName) function\n\n");

	/* Test Case 1: input less than 20 characters */
	uint8_t input1[] = "Less than twenty";	
	expected = WRONG_NAME; 

	result = getCardHolderName(&testCard);

	if (result == WRONG_NAME)
	{
		printf("Test case 1 passed!\n\n");
	}
	else
	{
		printf("Test case 1 failed!\n\n");
	}

	/* Test Case 2: input is twenty characters */
	uint8_t input2[] = "A twenty char phrase";
	expected = CARD_OK;

	result = getCardHolderName(&testCard);

	if (result == CARD_OK)
	{
		printf("Test case 2 passed!\n\n");
	}
	else
	{
		printf("Test case 2 failed!\n\n");
	}

	/* Test Case 3: input is twenty characters, but not all letters  */
	uint8_t input3[] = "A twenty-char phrase";	
	expected = WRONG_NAME;

	result = getCardHolderName(&testCard);

	if (result == WRONG_NAME)
	{
		printf("Test case 3 passed!\n\n");
	}
	else
	{
		printf("Test case 3 failed!\n\n");
	}

	/* Test Case 4: input is more than twenty and less than twenty four letters  */
	uint8_t input4[] = "twenty one characters";
	expected = CARD_OK;

	result = getCardHolderName(&testCard);

	if (result == CARD_OK)
	{
		printf("Test case 4 passed!\n\n");
	}
	else
	{
		printf("Test case 4 failed!\n\n");
	}

	/* Test Case 5: input is twenty four letters */
	uint8_t input5[] = "Random sentences to test";
	expected = CARD_OK;

	result = getCardHolderName(&testCard);

	if (result == CARD_OK)
	{
		printf("Test case 5 passed!\n\n");
	}
	else
	{
		printf("Test case 5 failed!\n\n");
	}

	/* Test Case 6: input is twenty five letters */
	uint8_t input6[] = "Random phrases for a test";
	expected = WRONG_NAME;

	result = getCardHolderName(&testCard);

	if (result == WRONG_NAME)
	{
		printf("Test case 6 passed!\n\n");
	}
	else
	{
		printf("Test case 6 failed!\n\n");
	}
}

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
