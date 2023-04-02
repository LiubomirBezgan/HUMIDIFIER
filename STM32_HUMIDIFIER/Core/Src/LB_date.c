/* Includes ------------------------------------------------------------------*/
#include <LB_date.h>
#include <stdio.h>
#include <stdbool.h>

/* Local data type -----------------------------------------------------------*/
typedef struct month {
	const char * month_abbreviation;
	uint8_t days;
} month_t;

/**
  * @brief  contains month abbreviation and the number of days in each month
  */
static const month_t whole_year[MONTHS] = {
	{"jan", 31},
	{"feb", 29},
	{"mar", 31},
	{"apr", 30},
	{"may", 31},
	{"jun", 30},
	{"jul", 31},
	{"aug", 31},
	{"sep", 30},
	{"oct", 31},
	{"nov", 30},
	{"dec", 31},
};

/* Program-specific declarations ---------------------------------------------*/
#define MAX_YEAR 9999
#define FIRST_DAY (1 == pdate->day)
#define LAST_DAY (whole_year[pdate->month_number].days == pdate->day)
#define LAST_DAY_FEB_NOT_LEAP ((whole_year[1].days - 1) <= pdate->day)
#define FIRST_MONTH (0 == pdate->month_number)
#define LAST_MONTH ( (MONTHS - 1) == pdate->month_number )

/* Local function prototype --------------------------------------------------*/
static bool LB_Leap_Year(Date_t * pdate);

/* Function definitions ------------------------------------------------------*/

/**
  * @brief  initializes a Date_t data type (date) and set the date of the library's latest update
  * @param  pdate points to the Date_t data type (date)
  * @retval None
  */
void LB_Init_Date(Date_t * pdate)
{
	pdate->day = CURRENT_DAY;
	pdate->month_number = CURRENT_MONTH - 1;
	pdate->year = CURRENT_YEAR;
}

/**
  * @brief  sets the next day of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Next_Day(Date_t * pdate)
{
	if (LAST_MONTH && LAST_DAY)
	{
		if (MAX_YEAR < ++(pdate->year))						// increment year
		{
			pdate->year = 0;
		}
		pdate->month_number = 0;							// set jan
		pdate->day = 1;										// set the first day of the month
	}
	else if (!LB_Leap_Year(pdate) && (1 == pdate->month_number) && LAST_DAY_FEB_NOT_LEAP)
	{
		pdate->month_number++;								// increment month
		pdate->day = 1;										// set the first day of the month
	}
	else if (LAST_DAY)
	{
		pdate->month_number++;								// increment month
		pdate->day = 1;										// set the first day of the month
	}
	else
	{
		pdate->day++;										// increment day
	}
}

/**
  * @brief  sets the previous day of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Prev_Day(Date_t * pdate)
{
	if (FIRST_MONTH && FIRST_DAY)
	{
		if (0 == (pdate->year)--)							// decrement year
		{
			pdate->year = MAX_YEAR;
		}
		pdate->month_number = (MONTHS - 1);					// set December
		pdate->day = whole_year[MONTHS - 1].days;			// set the last day of December
	}
	else if (!LB_Leap_Year(pdate) && (2 == pdate->month_number) && FIRST_DAY)
	{
		pdate->month_number--;								// decrement month
		pdate->day = (whole_year[pdate->month_number].days - 1);	// set the last day of February
	}
	else if (FIRST_DAY)
	{
		pdate->month_number--;								// decrement month
		pdate->day = whole_year[pdate->month_number].days;	// set the last day of the month
	}
	else
	{
		pdate->day--;										// decrement day
	}
}

/**
  * @brief  sets the next month of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Next_Month(Date_t * pdate)
{
	if ( (MONTHS - 1) < ++(pdate->month_number) )			// increment month
	{
		pdate->month_number = 0;
	}
	if (pdate->day > whole_year[pdate->month_number].days)
	{
		pdate->day = whole_year[pdate->month_number].days;
	}
}

/**
  * @brief  sets the previous month of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Prev_Month(Date_t * pdate)
{
	if ( 0 == (pdate->month_number)-- )						// decrement month
	{
		pdate->month_number = MONTHS - 1;
	}
	if (pdate->day > whole_year[pdate->month_number].days)
	{
		pdate->day = whole_year[pdate->month_number].days;
	}
}

/**
  * @brief  sets the next year of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Next_Year(Date_t * pdate)
{
	if (MAX_YEAR < ++(pdate->year))							// increment year
	{
		pdate->year = 0;
	}
	if (!LB_Leap_Year(pdate) && (1 == pdate->month_number) && LAST_DAY)
	{
		pdate->day = (whole_year[pdate->month_number].days - 1);
	}
}

/**
  * @brief  sets the previous year of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Prev_Year(Date_t * pdate)
{
	if (0 == (pdate->year)--)								// decrement year
	{
		pdate->year = MAX_YEAR;
	}
	if (!LB_Leap_Year(pdate) && (1 == pdate->month_number) && LAST_DAY)
	{
		pdate->day = (whole_year[pdate->month_number].days - 1);
	}
}

/**
  * @brief  sends formatted output to a string pointed to by the second argument
  * @param  pdate points to an initialized Date_t data type (date)
  * @param  message points to the target string
  * @retval returns the total number of characters written excluding the null-character
  * 		appended at the end of the string.
  */
int LB_Date_to_Str(Date_t * pdate, char * message)
{
	return sprintf(message, "%02u %s %04u", pdate->day, whole_year[pdate->month_number].month_abbreviation, pdate->year);
}

/* Local functions -----------------------------------------------------------*/

/**
  * @brief  Checks whether the year of Date_t data type (date) is a leap year or not.
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval returns true if the year is a leap year.
  */
static bool LB_Leap_Year(Date_t * pdate)
{
	if (0 == pdate->year % 4)
	{
		if (0 == pdate->year % 100)
		{
			if (0 == pdate->year % 400)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}
