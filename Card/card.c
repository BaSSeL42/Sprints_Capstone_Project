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

	char Expiry_String[MAX_SIZE] = "", temp[MAX_SIZE] = ""; // initilize two strings 
	printf("ENTER EXPIRY DATE IN THIS FORMAT MM/YY : ");
	fgets(Expiry_String, MAX_SIZE, stdin);// store expiry date in Expiry_String
	// check if input size is more or less and check on forward slash place
	if (strlen(Expiry_String) != MAX_EXPIRY || Expiry_String[FORWARD_SLASH_INDEX] != '/')return WRONG_EXP_DATE;
	strcpy(temp, Expiry_String);// if string in the right format store it in temp string

	const char s[FORWARD_SLASH_INDEX] = "/"; // create constant for FORWARD_SLASH 
	char* token;

	token = strtok(Expiry_String, s); // cut string into substrings with FORWARD_SLASH delimeter 
	uint8_t localIterator = ZERO; // initilze local counter with zero
	// while there is more tokens
	while (token != NULL) {
		// check on first token (Months)
		if (localIterator == ZERO && (atoi(token) > MAX_MONTH || atoi(token) < MIN_MONTH))return WRONG_EXP_DATE;
		token = strtok(NULL, s); // parse (years)
		localIterator++;
	}
	// if everything is okay take the temp string and store it in cardExpirationDate member 
	for (localIterator = ZERO; localIterator < MAX_EXPIRY; localIterator++)cardData->cardExpirationDate[localIterator] = temp[localIterator];
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

void getCardHolderNameTest(void)
{
	uint8_t expected, result;
	ST_cardData_t testCard = { {0},{0},{0} };

	printf("Tester Name: Alaa Hisham\n");
	printf("Function Name: getCardHolderName\n\n");

	/* Test Case 1: input less than 20 characters */
	printf("Test Case 1: input less than 20 characters\n");
	uint8_t input1[] = "Less than twenty";	
	expected = WRONG_NAME; 

	printf("\tInput: %s\n",input1);
	printf("\tExpected result: WRONG_NAME\n");

	result = getCardHolderName(&testCard);

	if (result == WRONG_NAME) {
		printf("\tActual result: WRONG_NAME\n\n");
	}
	else {
		("\tActual result: CARD_OK\n\n");
	}

	/* Test Case 2: input is twenty characters */
	printf("Test Case 2: input is twenty characters\n");
	uint8_t input2[] = "A twenty char phrase";
	expected = CARD_OK;

	printf("\tInput: %s\n", input2);
	printf("\tExpected result: CARD_OK\n");

	result = getCardHolderName(&testCard);

	if (result == CARD_OK)
	{
		printf("\tActual result: CARD_OK\n\n");
	}
	else
	{
		printf("\tActual result: WRONG_NAME\n\n");
	}

	/* Test Case 3: input is twenty characters, but not all letters */
	printf("Test Case 3: input is twenty characters, but not all letters\n");
	uint8_t input3[] = "A twenty-char phrase";	

	printf("\tInput: %s\n", input3);
	printf("\tExpected result: WRONG_NAME\n");

	result = getCardHolderName(&testCard);

	if (result == WRONG_NAME)
	{
		printf("\tActual result: WRONG_NAME\n\n");
	}
	else
	{
		printf("\tActual result: CARD_OK\n\n");
	}

	/* Test Case 4: input is more than twenty and less than twenty four letters */
	//printf("Test Case 4: input is more than twenty and less than twenty four letters\n");
	//uint8_t input4[] = "twenty one characters";
	//expected = CARD_OK;

	//printf("\tInput: %s\n", input4);
	//printf("\tExpected result: CARD_OK\n");

	//result = getCardHolderName(&testCard);

	//if (result == CARD_OK)
	//{
	//	printf("\tActual result: CARD_OK\n\n");
	//}
	//else
	//{
	//	printf("\tActual result: WRONG_NAME\n\n");
	//}

	///* Test Case 5: input is twenty four letters */
	//printf("Test Case 5: input is twenty four letters\n");
	//uint8_t input5[] = "Random sentences to test";
	//expected = CARD_OK;

	//printf("\tInput: %s\n", input5);
	//printf("\tExpected result: CARD_OK\n");

	//result = getCardHolderName(&testCard);

	//if (result == CARD_OK)
	//{
	//	printf("\tActual result: CARD_OK\n\n");
	//}
	//else
	//{
	//	printf("\tActual result: WRONG_NAME\n\n");
	//}

	/* Test Case 6: input is twenty five letters */
	printf("Test Case 4: input is twenty five letters\n");
	uint8_t input6[] = "Random phrases for a test";
	expected = WRONG_NAME;

	printf("\tInput: %s\n", input6);
	printf("\tExpected result: WRONG_NAME\n");

	result = getCardHolderName(&testCard);

	if (result == WRONG_NAME)
	{
		printf("\tActual result: WRONG_NAME\n\n");
	}
	else
	{
		printf("\tActual result: CARD_OK\n\n");
	}
}

void getCardExpiryDateTest(void) {

	ST_cardData_t x;
	EN_cardError_t returnedval;
	
	printf("Tester Name : Sharpel Malak\n");
	printf("Testcase 1  : check if input is null \n");
	printf("Input data  : \n");
	returnedval = getCardExpiryDate(&x);
	printf("Expected result : 2\n");
	printf("Actual result : %d\n", returnedval);
	
	printf("Tester Name : Sharpel Malak\n");
	printf("Testcase 2  : user enters less than 5 characters \n");
	printf("Input data  :  11/2\n");
	returnedval = getCardExpiryDate(&x);
	printf("Expected result : 2\n");
	printf("Actual result : %d\n", returnedval);
	
	printf("Tester Name : Sharpel Malak\n");
	printf("Testcase 3  : user enters more than 5 characters \n");
	printf("Input data  :  152/55\n");
	returnedval = getCardExpiryDate(&x);
	printf("Expected result : 2\n");
	printf("Actual result : %d\n", returnedval);
	
	printf("Tester Name : Sharpel Malak\n");
	printf("Testcase 4  : user enters wrong format and invalid month \n");
	printf("Input data  :  15/15\n");
	returnedval = getCardExpiryDate(&x);
	printf("Expected result : 2\n");
	printf("Actual result : %d\n", returnedval);
	
	printf("Tester Name : Sharpel Malak\n");
	printf("Testcase 5  : user right format \n");
	printf("Input data  :  11/25\n");
	returnedval = getCardExpiryDate(&x);
	printf("Expected result : 0\n");
	printf("Actual result : %d\n", returnedval);

}

void getCardPanTest(void)
{
	ST_cardData_t testCase1 = { "temp_name","temp_pan","date" };
	ST_cardData_t testCase2 = { "temp_name","temp_pan","date" };
	ST_cardData_t testCase3 = { "temp_name","temp_pan","date" };
	EN_cardError_t LOCAL_returnValue;
	printf("=========================================== \n\n");
	printf("Tester Name : Sherif Ashraf \n");
	printf("Test Case 1 : User Enter Right PAN  \n");
	printf("Input Data : 51102000115511100 \n");
	printf("Expected Result : CARD_OK \n");
	printf("Current primary Account Number : %s \n", testCase1.primaryAccountNumber);
	LOCAL_returnValue = getCardPan(&testCase1);
	if (LOCAL_returnValue == CARD_OK)
	{
		printf("Actual Result : CARD_OK \n");
	}
	else if (LOCAL_returnValue == WRONG_PAN)
	{
		printf("Actual Result : WRONG_PAN \n");
	}
	printf("New primary Account Number : %s \n", testCase1.primaryAccountNumber);
	printf("=========================================== \n\n");
	printf("\n\nTester Name : Sherif Ashraf \n");
	printf("Test Case 2 : User Enter PAN Less Than 16 Digit \n");
	printf("Input Data : 11020001155 \n");
	printf("Expected Result : WRONG_PAN \n");
	printf("Current primary Account Number : %s \n", testCase2.primaryAccountNumber);
	LOCAL_returnValue = getCardPan(&testCase2);
	if (LOCAL_returnValue == CARD_OK)
	{
		printf("Actual Result : CARD_OK \n");
	}
	else if (LOCAL_returnValue == WRONG_PAN)
	{
		printf("Actual Result : WRONG_PAN \n");
	}
	printf("No primary Account Number Will Be Stored : %s \n", testCase2.primaryAccountNumber);
	printf("=========================================== \n\n");
	printf("\n\nTester Name : Sherif Ashraf \n");
	printf("Test Case 3 : User Enter PAN Greater Than 20 Digit \n");
	printf("Input Data : 51102000115511100547862 \n");
	printf("Expected Result : WRONG_PAN \n");
	printf("Current primary Account Number : %s \n", testCase3.primaryAccountNumber);
	LOCAL_returnValue = getCardPan(&testCase3);
	if (LOCAL_returnValue == CARD_OK)
	{
		printf("Actual Result : CARD_OK \n");
	}
	else if (LOCAL_returnValue == WRONG_PAN)
	{
		printf("Actual Result : WRONG_PAN \n");
	}
	printf("No primary Account Number Will Be Stored : %s \n", testCase3.primaryAccountNumber);
	printf("=========================================== \n\n");
}
