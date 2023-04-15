/*
 * LB_SD_CARD_Humidifier.c
 *
 *  Created on: 15 apr 2023
 *      Author: SKG.POTOP
 */
/* Includes ------------------------------------------------------------------*/
#include <LB_SD_CARD_Humidifier.h>

/* Variables ------------------------------------------------------------------*/
// FATFS fs;						// file system TODO: check scope and linkage
extern const uint16_t logging_period[];

/* Function definitions ------------------------------------------------------*/
/** TODO: complete the description
  * @brief  initializes a Time_t data type (time)
  * @param  file_name points to the Time_t data type (time)
  * @param  pdate points to the Time_t data type (time)
  * @param  ptime
  * @param  dev
  * @param  data
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


/** TODO: complete the description
  * @brief  initializes a Time_t data type (time)
  * @param  file_name points to the Time_t data type (time)
  * @param  pdate points to the Time_t data type (time)
  * @param  ptime
  * @param  dev
  * @param  data
  * @retval ???
  */
FRESULT LB_update_logs(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data)
{
	FATFS fs;							// file system TODO: check scope and linkage
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
			fresult = f_mount(NULL, "", 1); // TODO: is it necessary?
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
		fresult = f_puts("Date,Time,Temperature [C],Humidity [%], Pressure [mmHg]\r\n", &fil);
	}

	// Move offset to the end of file
	fresult = f_lseek(&fil, /*fil.fptr*/ f_size(&fil));

	// Update THP data
	BME280_read_data(dev, data);

	/*** Updating an existing file ***/
	// Write a string to the file
	sprintf( (char *) message, "%04u-%02u-%02u,%02u:%02u:%02u,%.2f,%.2f,%.0f\n", pdate->year, (pdate->month_number + 1), pdate->day, ptime->time[2], ptime->time[1], ptime->time[0],  data->temperature, data->humidity, (data->pressure * 0.0075) );
	fresult = f_puts((TCHAR *) message, &fil);

	// Close the file
	f_close(&fil);

	// Unmount SD CARD
	fresult = f_mount(NULL, "", 1);

	return fresult;
}
