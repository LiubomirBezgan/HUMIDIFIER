/*
 * LB_SD_CARD_Humidifier.h
 *
 *  Created on: 15 apr 2023
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

// OLED
#define DATA_LOGGING_COLOR_TEXT WHITE
#define DATA_LOGGING_COLOR_DIGITS RED

/* Function prototypes -------------------------------------------------------*/

/** TODO: complete the description
  * @brief  initializes a Time_t data type (time)
  * @param  file_name points to the Time_t data type (time)
  * @param  pdate points to the Time_t data type (time)
  * @param  ptime
  * @param  dev
  * @param  data
  * @retval ???
  */
FRESULT LB_update_logs(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data);

/** TODO: complete the description
  * @brief  initializes a Time_t data type (time)
  * @param  file_name points to the Time_t data type (time)
  * @param  pdate points to the Time_t data type (time)
  * @param  ptime
  * @param  dev
  * @param  data
  * @retval None
  */
void LB_Data_Logging_Function(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data, uint16_t * delay_in_seconds, Data_Logging_Period_e period);
#endif /* INC_LB_SD_CARD_HUMIDIFIER_H_ */
