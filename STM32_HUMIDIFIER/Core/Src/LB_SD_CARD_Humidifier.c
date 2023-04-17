/*
 * LB_SD_CARD_Humidifier.c
 *
 *  Created on: Apr 15, 2023
 *      Author: SKG.POTOP
 */
/* Includes ------------------------------------------------------------------*/
#include <LB_SD_CARD_Humidifier.h>

/* Variables ------------------------------------------------------------------*/
const uint16_t logging_period[PERIOD_MAX] = {
		LOGGING_PERIOD_1, LOGGING_PERIOD_5, LOGGING_PERIOD_10,
		LOGGING_PERIOD_15, LOGGING_PERIOD_20, LOGGING_PERIOD_30
};

/* Function definitions ------------------------------------------------------*/
/**
  * @brief  creates a log file and write a date, time, temperature, humidity and pressure data with specified delay between consequent recording
  * @param  file_name points to string contains a log file name
  * @param  pdate points to the Date_t data type (date)
  * @param  ptime points to the Time_t data type (time
  * @param  dev points to the struct bme280_dev data type (bme280 device structure)
  * @param  pcomp_data points to the bme280_data structure, that contains the pressure, temperature and humidity data from the sensor
  * @param  delay_in_seconds points to the variable contains the current value of delay second counter
  * @param  period describes the index of logging period
  * @retval None
  */
void LB_Data_Logging_Function(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data, uint16_t * delay_in_seconds, Data_Logging_Period_e period)
{
	  if( logging_period[period] == (*delay_in_seconds) )
	  {
		  LB_update_logs(file_name, pdate, ptime, dev, data);
		  *delay_in_seconds = 0;
	  }
}


/**
  * @brief  creates a log file and write a date, time, temperature, humidity and pressure data
  * @param  file_name points to string contains log file name
  * @param  pdate points to the Date_t data type (date)
  * @param  ptime points to the Time_t data type (time
  * @param  dev points to the struct bme280_dev data type (bme280 device structure)
  * @param  pcomp_data points to the bme280_data structure, that contains the pressure, temperature and humidity data from the sensor
  * @retval File function return code (FRESULT) (see ff.h for details)
  */
FRESULT LB_update_logs(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data)
{
	FATFS fs;
	FIL fil;
	FRESULT fresult;
	uint8_t message[MAX_LEN];

	// Mount SD Card
	if ( FR_DISK_ERR == (fresult = f_mount(&fs, "", 1)) )
	{
		FATFS_UnLinkDriver(USERPath);
		MX_FATFS_Init();
		if ( FR_OK != (fresult = f_mount(&fs, "", 1)) )
		{
			fresult = f_mount(NULL, "", 1);
			return fresult;
		}
	}

	// Open the file with write access
	if ( FR_OK != (fresult = f_open(&fil, file_name, FA_OPEN_EXISTING | FA_WRITE)) )
	{
		if ( FR_OK != (fresult = f_open(&fil, file_name, FA_OPEN_ALWAYS | FA_WRITE)) )
		{
			// Unmount SD CARD
			fresult = f_mount(NULL, "", 1);
			return fresult;
		}
		#ifdef CSV_LOGGING
		fresult = f_puts("Date,Time,Temperature [C],Humidity [%], Pressure [mmHg]\r\n", &fil);
		#else
		fresult = f_puts("Date,Time,Temperature [C],Humidity [%], Pressure [mmHg]\r\n\r\n", &fil);
		#endif
	}

	// Move offset to the end of file
	fresult = f_lseek(&fil, /*fil.fptr*/ f_size(&fil));

	// Update THP data
	BME280_read_data(dev, data);

	/*** Updating an existing file ***/
	// Write a string to the file
	#ifdef CSV_LOGGING
	sprintf( (char *) message, "%04u-%02u-%02u,%02u:%02u:%02u,%.2f,%.2f,%.0f\n", pdate->year, (pdate->month_number + 1), pdate->day, ptime->time[2], ptime->time[1], ptime->time[0],  data->temperature, data->humidity, (data->pressure * 0.0075) );
	#else
	sprintf( (char *) message, "%04u-%02u-%02u,%02u:%02u:%02u,%.2f,%.2f,%.0f\r\n", pdate->year, (pdate->month_number + 1), pdate->day, ptime->time[2], ptime->time[1], ptime->time[0],  data->temperature, data->humidity, (data->pressure * 0.0075) );
	#endif
	fresult = f_puts((TCHAR *) message, &fil);

	// Close the file
	f_close(&fil);

	// Unmount SD CARD
	fresult = f_mount(NULL, "", 1);

	return fresult;
}
