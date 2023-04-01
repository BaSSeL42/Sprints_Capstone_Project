/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/

#include "card.h"
#pragma warning(disable : 4996)


/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint8_t size, Local_Iterator=0;

	/* Get name from user */
	printf("Card Holder Name: ");
	scanf("%[^\n]%*c", cardData->cardHolderName);
	//printf("%s\n", cardData->cardHolderName);
	
	/* Check whether the name length is within limits */
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


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	char str[MAX_SIZE] = "", temp[MAX_SIZE] = "";
	printf("ENTER EXPIRE DATE IN THIS FORMAT MM/YY : ");
	fgets(str, MAX_SIZE, stdin);
	if (strlen(str) != MAX_EXPIRE || str[2] != '/')return WRONG_EXP_DATE;
	strcpy(temp, str);
	const char s[2] = "/";
	char* token;

	token = strtok(str, s);
	int i = 0;
	while (token != NULL) {
		if (i == 0 && (atoi(token) > MAX_MONTH || atoi(token) < MIN_MONTH))return WRONG_EXP_DATE;
		token = strtok(NULL, s);
		i++;
	}

	for (i = 0; i < 6; i++)cardData->cardExpirationDate[i] = temp[i];
	return CARD_OK;

}

/*

- Description : This Function Ask User To Enter His PAN Number And Check It Then Store It In The Card Data
- Paramters   : It Take One Paramter From ST_cardData_t Data Type 
- Return	  : It Return EN_cardError_t Data Type 
				CARD_OK If Every Thing Is Ok
				WRONG_PAN If PAN Number Cant Pass The Checker
*/

EN_cardError_t getCardPan(ST_cardData_t* cardData)
{
	EN_cardError_t LOCAL_returnValue = WRONG_PAN;
	uint8_t LOCAL_checkPan[PAN_ARRAY_MAX_SIZE] = "NULL";
	uint8_t LOCAL_counter,LOCAL_flag = 0;

	/* Scan PAN From User */
	printf("Please Enter Your Primary Account Number \n");
	scanf("%s",LOCAL_checkPan);

	/*Loop Until The End Of The String*/
	for(uint8_t LOCAL_counter = 0 ; LOCAL_counter < strlen(LOCAL_checkPan);LOCAL_counter++)
	{
		/* Check That All Index Is Digits */
		if(isdigit(LOCAL_checkPan[LOCAL_counter]))
		{
			/* Increment Flag With One To Compare Number Of Digits In The Array With It All Elemant*/
			LOCAL_flag++;
		}
	}
	/* Check If The String Length Not Equal Null Or Smaller Than Min Size Or Gearter Than Max Size And 
	   LOCAL_FLAG Not Equal String Length */
	if( (strlen(LOCAL_checkPan) == NULL ) || (strlen(LOCAL_checkPan) < PAN_MIN_SIZE) || (strlen(LOCAL_checkPan) > PAN_MAX_SIZE)
		 || (LOCAL_flag != (strlen(LOCAL_checkPan))))
	{
		/* Return WRONG_PAN */
		LOCAL_returnValue = WRONG_PAN;
	}
	else
	{
		/* Else Return CARD_OK Then Copy PAN To cardData Struct  */
		LOCAL_returnValue = CARD_OK;
		strcpy(cardData->primaryAccountNumber,LOCAL_checkPan);
	}
	return LOCAL_returnValue;
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

void getCardExpiryDateTest(void) {

	ST_cardData_t x;
	EN_cardError_t returnedval;
	returnedval = getCardExpiryDate(&x);
	printf("Tester Name : Sharpel Malak\n");
	printf("Testcase 5  : user enters wrong format \n");
	printf("Input data  :  15215\n");
	printf("Expected result : 2\n");
	printf("Actual result : %d\n", returnedval);

}

void getCardPanTest(void)
{
	 ST_cardData_t testCase1 =  {"temp","temp","temp"};
	 ST_cardData_t testCase2 =  {"temp","temp","temp"};
	 ST_cardData_t testCase3 = {"temp","temp","temp"};
	 EN_cardError_t LOCAL_returnValue;
	printf("Tester Name : Sherif Ashraf \n");
	printf("Test Case 1 : \n");
	printf("Input Data : 51102000115511100 \n");
	printf("Expected Result : 0 \n");
	printf("Current primary Account Number : %s \n",testCase1.primaryAccountNumber);
	LOCAL_returnValue = getCardPan(&testCase1);
	printf("Actual Result : %d \n",LOCAL_returnValue);
	printf("Stored primary Account Number : %s \n",testCase1.primaryAccountNumber);

	printf("\n\nTester Name : Sherif Ashraf \n");
	printf("Test Case 2 : \n");
	printf("Input Data : 11020001155 \n");
	printf("Expected Result : 3 \n");
	printf("Current primary Account Number : %s \n",testCase2.primaryAccountNumber);
	LOCAL_returnValue = getCardPan(&testCase2);
	printf("Actual Result : %d \n",LOCAL_returnValue);
	printf("Stored primary Account Number : %s \n",testCase2.primaryAccountNumber);

	printf("\n\nTester Name : Sherif Ashraf \n");
	printf("Test Case 3 : \n");
	printf("Input Data : 51102000115511100547862 \n");
	printf("Expected Result : 3 \n");
	printf("Current primary Account Number : %s \n",testCase3.primaryAccountNumber);
	LOCAL_returnValue = getCardPan(&testCase3);
	printf("Actual Result : %d \n",LOCAL_returnValue);
	printf("Stored primary Account Number : %s \n",testCase3.primaryAccountNumber);
}
