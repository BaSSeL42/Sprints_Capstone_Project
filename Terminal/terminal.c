/**************************************************************************************************************************
 *                                                      Includes
**************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Common/STD_types.h"
#include "../Card/card.h"
#include "terminal.h"

#pragma warning(disable : 4996)

/**************************************************************************************************************************
 *                                              Functions Implementation
**************************************************************************************************************************/
/**
 * @author  : Bassel Yasser
 * @brief   : Get Transaction date from user
 * @param   : ST_terminalData_t *termData 
 * @return  : EN_terminalError_t {TERMINAL_OK or WRONG_DATE}
*/
EN_terminalError_t getTransactionDate( ST_terminalData_t *termData )
{
    EN_terminalError_t errorStatus;
    uint8_t transDate[50];
    uint8_t buffer[50];
    int32_t x, flag = 0;

    printf("Please Enter Transaction Date: ");
    scanf("%s", transDate);
    strcpy( buffer, transDate );

    uint8_t* token = strtok(buffer, "/");
    while(token != NULL)
    {
        x = atoi(token);
        if ( flag == 0  )
        {
            if ( x > 0 && x <= 31 )
            {
                flag = 1;
            }
            else 
            {
                break;
            }
        }
        else if (flag == 1)
        {
            if ( x >= 1 && x <= 12 )
            {
                flag = 2;
            }
            else 
            {
                break;
            }
        }
        else if ( flag == 2 )
        {
            if ( x > 2000 && x < 2100 )
            {
                flag = 3;
            }
            else 
            {
                break;
            }
        }
        token = strtok(NULL, "/");
    }

    
    
    if(termData == NULL || strlen(transDate) < 10 || flag != 3)
    {
        errorStatus = WRONG_DATE;
    }
    else 
    {
        errorStatus = TERMINAL_OK;
        strcpy( termData->transactionDate, transDate );
    }


    return errorStatus;
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
		else if (cardData->cardExpirationDate[EXP_DATE_Y_UNITS] == termData->transactionDate[TRANS_DATE_Y_UNITS])
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


EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
    float amount; // create local float amount
    printf("ENTER THE TRANSACTION AMOUNT : ");
    scanf("%f", &amount);
    if (amount <= 0)return INVALID_AMOUNT; // if value less than or equal zero --> wrong
    termData->transAmount = amount; // store amount in transAmount member
    return TERMINAL_OK;


}

/*

- Description : This Function Compare The Transaction Amount With The Max Allowed Terminal Amount
- Paramters   : It Take One Paramter From ST_terminalData_t Data Type
- Return	  : It Return EN_terminalError_t Data Type
                Terminal_OK If Every Thing Is Ok
                EXCEED_MAX_ACCOUNT If PAN Number Cant Pass The Checker
*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData )
{
    EN_terminalError_t LOCAL_returnValue = EXCEED_MAX_AMOUNT;

    /*  Set The Max Amount Of The Transaction With The Terminal Max Amount*/
    termData->maxTransAmount = MAX_TRANS_AMOUNT;

    /* Check If The Transaction Amount Greater Than The Max Transaction Amount */
    if(termData->transAmount > termData->maxTransAmount)
    {
        /* Return EXCEED_MAX_AMOUNT */
        LOCAL_returnValue = EXCEED_MAX_AMOUNT;
    }
    else
    {
        /* Else Return TERMINAL_OK */
        LOCAL_returnValue = TERMINAL_OK;
    }
    return LOCAL_returnValue;
}


/**
 * @author  : Bassel Yasser
 * @brief   : Set Maximum Amount 
 * @param   : ST_terminalData_t *termData 
 * @param   : float maxAmount
 * @return  : EN_terminalError_t {TERMINAL_OK or INVALID_MAX_AMOUNT}
*/
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData , float maxAmount)
{
    EN_terminalError_t errorStatus;
    if (termData != NULL)
    {
        if ( maxAmount > 0 )
        {
            errorStatus = TERMINAL_OK;
            termData->maxTransAmount = maxAmount;
        }
        else if ( maxAmount <= 0 )
        {
            errorStatus = INVALID_MAX_AMOUNT;
        }
        else 
        {
            /*do nothing*/
        }
    }
    else 
    {
        /* do nothing */
    }

    return errorStatus;
}


EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
	int8_t LocalIterator, PAN_sum = 0, char2num;
	uint8_t PAN_length = strlen(cardData->primaryAccountNumber);
	uint8_t second = 0;

    /* Start from the last element in the PAN */
	for (LocalIterator = PAN_length-1; LocalIterator >= 0; LocalIterator--)
	{
        /* Convert digit from character to integer */
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
		/* Toggle to operate on every other element */
		second ^= 1;
	}

    /**/
	if (PAN_sum % 10 != 0) return INVALID_CARD;

	return TERMINAL_OK;
}





/**************************************************************************************************************************
 *                                                  Function Implementation For Testing
**************************************************************************************************************************/
ST_terminalData_t terminalData;
int32_t ret;

void getTransactionDateTest(void)
{
    printf("Test Case1 \n");
    printf("Input: \"25/03/2010\" \n");

    printf(" expected output : Ok\n");

    ret = getTransactionDate( &terminalData );
    if (ret == TERMINAL_OK)
    {
        printf("Actual result: OK \n");
    }
    else if( ret == WRONG_DATE )
    {
        printf("WRONG_DATE\n");
    }

    printf("Test Case2 \n");
    printf("Input: \"255/3/2010\" \n");

    printf(" expected output : wrong Date\n");

    ret = getTransactionDate( &terminalData );
    if (ret == TERMINAL_OK)
    {
        printf("Actual result: OK \n");
    }
    else if( ret == WRONG_DATE )
    {
        printf("WRONG_DATE\n");
    }
}


void isCardExpiredTest(void)
{
	uint8_t expected, result;
	ST_cardData_t cardData = { {0},{0},"11/22" };

    printf("Tester Name: Alaa Hisham\n");
    printf("Function Name: isCardExpired\n\n");
    printf("Expiry date: 11/22\n\n");

	/*----------------- Test Case 1 ------------------*/
	ST_terminalData_t termData1 = { 0,0, "30/12/2019" };
	expected = TERMINAL_OK;

    printf("Test Case 1\n");
    printf("\tTransaction date: %s\n", "30/12/2019");
    printf("\tExpected result: TERMINAL_OK\n");

	result = isCardExpired(&cardData, &termData1);

	if (result == TERMINAL_OK)
	{
        printf("\tActual result: TERMINAL_OK\n\n");
	}
	else
	{
        printf("\tActual result: EXPIRED_CARD\n\n");
	}

	/*----------------- Test Case 2 ------------------*/
	ST_terminalData_t termData2 = { 0,0, "30/03/2022" };
	expected = TERMINAL_OK;

    printf("Test Case 2\n");
    printf("\tTransaction date: 30/03/2022\n");
    printf("\tExpected result: TERMINAL_OK\n");

	result = isCardExpired(&cardData, &termData2);

	if (result == TERMINAL_OK)
	{
        printf("\tActual result: TERMINAL_OK\n\n");
	}
	else
	{
        printf("\tActual result: EXPIRED_CARD\n\n");
	}

	/*----------------- Test Case 3 ------------------*/
	ST_terminalData_t termData3 = { 0,0, "30/12/2022" };
	expected = EXPIRED_CARD;

    printf("Test Case 3\n");
    printf("\tTransaction date: %s\n", "30/12/2022");
    printf("\tExpected result: EXPIRED_CARD\n");

	result = isCardExpired(&cardData, &termData3);

	if (result == EXPIRED_CARD)
	{
		printf("\tActual result: EXPIRED_CARD\n\n");
	}
	else
	{
		printf("\tActual result: TERMINAL_OK\n\n");
	}
}



void setMaxAmountTest (void)
{
    printf("Test Case1 \n");
    printf("Input: 10000.0 \n");

    printf(" expected output : Ok\n");

    ret = setMaxAmount( &terminalData,  10000.0 );

    if (ret == TERMINAL_OK)
    {
        printf("Actual result: OK \n");
    }
    else if( ret == INVALID_MAX_AMOUNT )
    {
        printf("Actual result: INVALID_MAX_AMOUNT\n");
    }
    printf("..........................................................................\n\n");
    printf("Test Case2 \n");
    printf("Input: -1200 \n");

    printf(" expected output : Invalid Max Amount\n");

    ret = setMaxAmount( &terminalData,  -1200 );

    if (ret == TERMINAL_OK)
    {
        printf("Actual result: OK \n");
    }
    else if( ret == INVALID_MAX_AMOUNT )
    {
        printf("Actual result: INVALID_MAX_AMOUNT\n");
    }

}


void getTransactionAmountTest(void) {
    ST_terminalData_t Terminal_Data;
    EN_terminalError_t returnedval;

    
   
    printf("Tester Name : Sharpel Malak\n");
    printf("Testcase 1  : user enters zero or less amount \n");
    printf("Input data  :  -100\n");
    returnedval = getTransactionAmount(&Terminal_Data);
    printf("Expected result : 4\n");
    printf("Actual result : %d\n", returnedval);
    
    printf("Tester Name : Sharpel Malak\n");
    printf("Testcase 2  : user enters postive number \n");
    printf("Input data  :  200\n");
    returnedval = getTransactionAmount(&Terminal_Data);
    printf("Expected result : 0\n");
    printf("Actual result : %d\n", returnedval);

}


void isValidCardPANTest(void)
{
	uint8_t expected, result;

    printf("Tester Name: Alaa Hisham\n");
    printf("Function Name: isValidCardPAN\n\n");

	/*----------------- Test Case 1 ------------------*/
	/* Not a valid card number but it satisfies luhn's algorithm */
	ST_cardData_t cardData1 = { {0}, "8649", {0} };
	expected = TERMINAL_OK;

    printf("Test Case 1\n");
    printf("\tInput: %s\n", "8649");
    printf("\tExpected result: TERMINAL_OK\n");

	result = isValidCardPAN(&cardData1);

	if (result == TERMINAL_OK)
	{
		printf("\tActual result: TERMINAL_OK\n\n");
	}
	else
	{
        printf("\tActual result: INVALID_CARD\n\n");
	}

	/*----------------- Test Case 2 ------------------*/
	ST_cardData_t cardData2 = { {0}, "432420754971832789", {0} };
	expected = INVALID_CARD;

    printf("Test Case 2\n");
    printf("\tInput: %s\n", "432420754971832789");
    printf("\tExpected result: INVALID_CARD\n");

	result = isValidCardPAN(&cardData2);

	if (result == INVALID_CARD)
	{
		printf("\tActual result: INVALID_CARD\n\n");
	}
	else
	{
        printf("\tActual result: TERMINAL_OK\n\n");
	}

	/*----------------- Test Case 3 ------------------*/
	ST_cardData_t cardData3 = { {0}, "4342077277183288", {0} };
	expected = TERMINAL_OK;

    printf("Test Case 3\n");
    printf("\tInput: %s\n", "4342077277183288");
    printf("\tExpected result: TERMINAL_OK\n");

	result = isValidCardPAN(&cardData3);

	if (result == TERMINAL_OK)
	{
		printf("\tActual result: TERMINAL_OK\n\n");
	}
	else
	{
        printf("\tActual result: INVALID_CARD\n\n");
	}

	/*----------------- Test Case 4 ------------------*/
	ST_cardData_t cardData4 = { {0}, "1234567898765432", {0} };
	expected = INVALID_CARD;

    printf("Test Case 4\n");
    printf("\tInput: %s\n", "1234567898765432");
    printf("\tExpected result: INVALID_CARD\n");

	result = isValidCardPAN(&cardData4);

	if (result == INVALID_CARD)
	{
		printf("\tActual result: INVALID_CARD\n\n");
	}
	else
	{
        printf("\tActual result: TERMINAL_OK\n\n");
	}

}

void isBelowMaxAmountTest(void)
{
    ST_terminalData_t testCase1 = {500,10000,"temp"};
    ST_terminalData_t testCase2 = {11000,10000,"temp"};
    EN_terminalError_t LOCAL_returnValue;

    printf("=========================================== \n\n");
    printf("Tester Name : Sherif Ashraf \n");
    printf("Test Case 1 : TransAmount Less Than The Max Amount \n");
    printf("Input Data : transAmount = 500 , maxAccount = 10000 \n");
    printf("Expected Result : TERMINAL_OK \n");
    LOCAL_returnValue = isBelowMaxAmount(&testCase1);
    if (LOCAL_returnValue == TERMINAL_OK)
    {
        printf("Actual Result : TERMINAL_OK \n");
    }
    else if(LOCAL_returnValue == EXCEED_MAX_AMOUNT)
    {
        printf("Actual Result : EXCEED_MAX_AMOUNT \n");
    }
    printf("=========================================== \n\n");
    printf("Tester Name : Sherif Ashraf \n");
    printf("Test Case 2 : TransAmount More Than The Max Amount\n");
    printf("Input Data : transAmount = 11000 , maxAccount = 10000 \n");
    printf("Expected Result : EXCEED_MAX_AMOUNT \n");
    LOCAL_returnValue = isBelowMaxAmount(&testCase2);
    if(LOCAL_returnValue == TERMINAL_OK)
    {
        printf("Actual Result : TERMINAL_OK \n");
    }
    else if(LOCAL_returnValue == EXCEED_MAX_AMOUNT)
    {
        printf("Actual Result : EXCEED_MAX_AMOUNT \n");
    }
    printf("=========================================== \n\n");
}