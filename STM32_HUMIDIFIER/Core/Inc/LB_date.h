//  LB_date.h
//  v1.1

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LB_DATE_H_
#define _LB_DATE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Program-specific declarations ---------------------------------------------*/
#define MONTHS 12

// set the current date
#define CURRENT_DAY 16
#define CURRENT_MONTH 4		// a number from 1 to 12
#define CURRENT_YEAR 2023

/* General type definitions --------------------------------------------------*/
/* redefine Date_t as appropriate */
typedef struct day_m_y {
	uint8_t day;
	uint8_t month_number;
	uint16_t year;
} Date_t;

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  initializes a Date_t data type (date) and set the date of the library's latest update
  * @param  pdate points to the Date_t data type (date)
  * @retval None
  */
void LB_Init_Date(Date_t * pdate);

/**
  * @brief  sets the next day of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Next_Day(Date_t * pdate);

/**
  * @brief  sets the previous day of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Prev_Day(Date_t * pdate);

/**
  * @brief  sets the next month of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Next_Month(Date_t * pdate);

/**
  * @brief  sets the previous month of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Prev_Month(Date_t * pdate);

/**
  * @brief  sets the next year of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Next_Year(Date_t * pdate);

/**
  * @brief  sets the previous year of Date_t data type (date)
  * @param  pdate points to an initialized Date_t data type (date)
  * @retval None
  */
void LB_Prev_Year(Date_t * pdate);

/**
  * @brief  sends formatted output to a string pointed to by the second argument
  * @param  pdate points to an initialized Date_t data type (date)
  * @param  message points to the target string
  * @retval returns the total number of characters written excluding the null-character
  * 		appended at the end of the string.
  */
int LB_Date_to_Str(Date_t * pdate, char * message);

#endif
