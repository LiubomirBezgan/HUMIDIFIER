/*
 * LB_SD_CARD_Humidifier.h
 *
 *  Created on: Apr 15, 2023
 *      Author: SKG.POTOP
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_LB_SD_CARD_HUMIDIFIER_H_
#define INC_LB_SD_CARD_HUMIDIFIER_H_

/* Includes ------------------------------------------------------------------*/
#include "fatfs.h"
#include "fatfs_sd.h"
#include "diskio.h"

// Humidity sensor
#include "bme280_defs.h"

// Date and time
#include "LB_date.h"
#include "LB_time.h"

// FMS
#include "LB_FSM_Humidifier.h"

/* Program-specific declarations ---------------------------------------------*/
#define MAX_LEN 37
#define MAX_SD_CARD_BUFF 1024
#define LOGGING_PERIOD_1 60			// 1 min
#define LOGGING_PERIOD_5 300		// 5 min
#define LOGGING_PERIOD_10 600		// 10 min
#define LOGGING_PERIOD_15 900		// 15 min
#define LOGGING_PERIOD_20 1200		// 20 min
#define LOGGING_PERIOD_30 1800		// 30 min

// The type of log files (switch .csv/.txt)
#define CSV_LOGGING // comment the line if you want to write to a .txt file

/* General type definitions --------------------------------------------------*/
// SD CARD
typedef enum {
	logging_1_min,
	logging_5_min,
	logging_10_min,
	logging_15_min,
	logging_20_min,
	logging_30_min,
	PERIOD_MAX
} Data_Logging_Period_e;

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  creates a log file and write a date, time, temperature, humidity and pressure data
  * @param  file_name points to string contains log file name
  * @param  pdate points to the Date_t data type (date)
  * @param  ptime points to the Time_t data type (time
  * @param  dev points to the struct bme280_dev data type (bme280 device structure)
  * @param  pcomp_data points to the bme280_data structure that contains the pressure, temperature and humidity data from the sensor
  * @retval File function return code (FRESULT) (see ff.h for details)
  */
FRESULT LB_update_logs(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data);

/**
  * @brief  creates a log file and write a date, time, temperature, humidity and pressure data with specified delay between consequent recording
  * @param  file_name points to string contains a log file name
  * @param  pdate points to the Date_t data type (date)
  * @param  ptime points to the Time_t data type (time
  * @param  dev points to the struct bme280_dev data type (bme280 device structure)
  * @param  pcomp_data points to the bme280_data structure that contains the pressure, temperature and humidity data from the sensor
  * @param  delay_in_seconds points to the variable contains the current value of delay second counter
  * @param  period describes the index of logging period
  * @retval None
  */
void LB_Data_Logging_Function(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data, uint16_t * delay_in_seconds, Data_Logging_Period_e period);
#endif /* INC_LB_SD_CARD_HUMIDIFIER_H_ */
