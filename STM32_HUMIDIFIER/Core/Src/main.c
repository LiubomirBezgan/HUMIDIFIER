/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// Date and time
#include "LB_date.h"
#include "LB_time.h"

// Humidity sensor
#include "bme280_add.h"
#include "bme280_defs.h"

// OLED
#include "SSD1331.h"
#include "LB_OLED_Humidifier.h"

// SD CARD
#include "fatfs_sd.h"

// UI
#include "LB_UI_Joystick.h"

// FMS
#include "LB_FSM_Humidifier.h"

// Ultrasonic membrane humidifier
#include "LB_USM_Humidifier.h"

// Generic
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
// TODO: disable the UART2
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Data output
#define MAX_LEN 37

// SD CARD
#define MAX_SD_CARD_BUFF 1024
#define LOGGING_PERIOD_1 60			// 1 min
#define LOGGING_PERIOD_5 300		// 5 min
#define LOGGING_PERIOD_10 600		// 10 min
#define LOGGING_PERIOD_15 900		// 15 min
#define LOGGING_PERIOD_20 1200		// 20 min
#define LOGGING_PERIOD_30 1800		// 30 min

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// Ultrasonic membrane humidifier
USM_Humidifier_settings_t Membrane_parameters = {hum_lvl_50, hum_duration_10, hum_delay_5};
Membrane_t USM_Humidifier;

// Date and time
Time_t time;
Date_t today;

// Temperature, humidity and pressure measurement
struct bme280_dev bme280_sens_dev;
struct bme280_data bme280_sens_data;
struct bme280_data bme280_sens_logging_data;
UNITS_e unit_system = SI;

// OLED
uint8_t thp_screen_counter = 0;
volatile bool clock_screen_update = false;

// SD CARD
Data_Logging_Period_e logging_index = logging_1_min;
const uint16_t logging_period[PERIOD_MAX] = {
		LOGGING_PERIOD_1, LOGGING_PERIOD_5, LOGGING_PERIOD_10,
		LOGGING_PERIOD_15, LOGGING_PERIOD_20, LOGGING_PERIOD_30
};
uint16_t logging_counter = 0;
char * logs_file_name = "logs.csv"; //char * logs_file_name = "logs.txt";

FATFS fs;							// file system
FIL fil;							// file
FRESULT fresult;					// to store the result
char buffer[MAX_SD_CARD_BUFF];		// to store data
UINT br, bw;						// file read/write count
// capacity related variables
FATFS * pfs;
DWORD fre_clust;
uint32_t total, free_space;

// UI
Joystick_t Joystick;
Button_t Joystick_Moved;

// FSM
STATE_e FSM_State = state_thp_screen;
EVENT_e FSM_Event = event_none;

TRANSITION_FUNC_PTR_t LB_Transition_Table[STATE_MAX][EVENT_MAX] = {
		[state_thp_screen]					[event_none]			=thp_screen,
		[state_thp_screen]					[event_button_pressed]	=thp_screen,
		[state_thp_screen]					[event_joystick_right]	=humidifier_screen,
		[state_thp_screen]					[event_joystick_left]	=data_logging_period_screen,

		[state_humidifier_screen]			[event_none]			=humidifier_screen,
		[state_humidifier_screen]			[event_button_pressed]	=set_humidity,
		[state_humidifier_screen]			[event_joystick_right]	=clock_screen,
		[state_humidifier_screen]			[event_joystick_left]	=thp_screen,

		[state_clock_screen]				[event_none]			=clock_screen,
		[state_clock_screen]				[event_button_pressed]	=set_time_h,
		[state_clock_screen]				[event_joystick_right]	=data_logging_period_screen,
		[state_clock_screen]				[event_joystick_left]	=humidifier_screen,

		[state_data_logging_period_screen]	[event_none]			=data_logging_period_screen,
		[state_data_logging_period_screen]	[event_button_pressed]	=set_data_logging_period,
		[state_data_logging_period_screen]	[event_joystick_right]	=thp_screen,
		[state_data_logging_period_screen]	[event_joystick_left]	=clock_screen,

		[state_set_humidity]				[event_none]			=set_humidity,
		[state_set_humidity]				[event_button_pressed]	=set_duration,
		[state_set_humidity]				[event_joystick_right]	=set_humidity,
		[state_set_humidity]				[event_joystick_left]	=set_humidity,

		[state_set_duration]				[event_none]			=set_duration,
		[state_set_duration]				[event_button_pressed]	=set_delay,
		[state_set_duration]				[event_joystick_right]	=set_duration,
		[state_set_duration]				[event_joystick_left]	=set_duration,

		[state_set_delay]					[event_none]			=set_delay,
		[state_set_delay]					[event_button_pressed]	=humidifier_screen,
		[state_set_delay]					[event_joystick_right]	=set_delay,
		[state_set_delay]					[event_joystick_left]	=set_delay,

		[state_set_time_h]					[event_none]			=set_time_h,
		[state_set_time_h]					[event_button_pressed]	=set_date_d,
		[state_set_time_h]					[event_joystick_right]	=set_time_m,
		[state_set_time_h]					[event_joystick_left]	=set_time_s,

		[state_set_time_m]					[event_none]			=set_time_m,
		[state_set_time_m]					[event_button_pressed]	=set_date_d,
		[state_set_time_m]					[event_joystick_right]	=set_time_s,
		[state_set_time_m]					[event_joystick_left]	=set_time_h,

		[state_set_time_s]					[event_none]			=set_time_s,
		[state_set_time_s]					[event_button_pressed]	=set_date_d,
		[state_set_time_s]					[event_joystick_right]	=set_time_h,
		[state_set_time_s]					[event_joystick_left]	=set_time_m,

		[state_set_date_y]					[event_none]			=set_date_y,
		[state_set_date_y]					[event_button_pressed]	=clock_screen,
		[state_set_date_y]					[event_joystick_right]	=set_date_d,
		[state_set_date_y]					[event_joystick_left]	=set_date_m,

		[state_set_date_m]					[event_none]			=set_date_m,
		[state_set_date_m]					[event_button_pressed]	=clock_screen,
		[state_set_date_m]					[event_joystick_right]	=set_date_y,
		[state_set_date_m]					[event_joystick_left]	=set_date_d,

		[state_set_date_d]					[event_none]			=set_date_d,
		[state_set_date_d]					[event_button_pressed]	=clock_screen,
		[state_set_date_d]					[event_joystick_right]	=set_date_m,
		[state_set_date_d]					[event_joystick_left]	=set_date_y,

		[state_set_data_logging_period]		[event_none]			=set_data_logging_period,
		[state_set_data_logging_period]		[event_button_pressed]	=data_logging_period_screen,
		[state_set_data_logging_period]		[event_joystick_right]	=set_data_logging_period,
		[state_set_data_logging_period]		[event_joystick_left]	=set_data_logging_period

};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
// UI
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

// TIM
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

// SD CARD
//void send_uart (char * string);	// to send the data to the uart
//int bufsize (char * buf);		// to find the size of data in the buffer
//void bufclear (void);			// to clear the buffer
FRESULT LB_update_logs(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data);
void LB_Data_Logging_Function(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data, uint16_t * delay_in_seconds, Data_Logging_Period_e period);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_I2C1_Init();
  MX_SPI3_Init();
  MX_TIM10_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Initializations of date and time
  LB_Init_Date(&today);
  LB_Init_Time(&time);
  LB_Set_Time(&time, 12, 0, 0);

  // The initialization of UI
  LB_Init_Button(&Joystick_Moved);
  LB_ADC_Start_DMA(&hadc1, &Joystick);

  // The initialization of humidity sensor
  BME280_init(&bme280_sens_dev);

  // The initialization of OLED
  ssd1331_init();
  ssd1331_clear_screen(BACKGROUND_COLOR);

  //   // The initialization of TIM10
  HAL_Delay(1000);
  HAL_TIM_Base_Start_IT(&htim10);

  // Mount SD Card
  //fresult = f_mount(&fs, "", 0);
  /*if (FR_OK != fresult)
  {
	  send_uart("error in mounting SD CARD...\r\n");
  }
  else
  {
	  send_uart("SD CARD mounted successfully...\r\n");
  }*/

  /*** Card capacity details ***/

  // check free space
  /*f_getfree("", &fre_clust, &pfs);

  total = (uint32_t) ( (pfs->n_fatent - 2) * pfs->csize * 0.5);
  sprintf(buffer, "SD CARD Total Size: \t%lu\r\n", total);
  send_uart(buffer);
  bufclear();
  free_space = (uint32_t) (fre_clust * pfs->csize * 0.5);
  sprintf(buffer, "SD CARD Free Space: \t%lu\r\n", free_space);
  send_uart(buffer);*/

  /*** The following operation is using PUTS and GETS ***/

  // Open file to write/ create a file if it doesn't exist
  //fresult = f_open(&fil, logs_file_name, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);	// TODO: read about f_open() macros

  // Writing text
  //fresult = f_puts("Date,Time,Temperature [C],Humidity [%], Pressure [mmHg]\r\n\n\r", &fil);
  // TODO: prepare macros for logs type (switch .csv/.txt)
  //send_uart("logs.csv created and the data is written.\r\n"); // send_uart("logs.txt created and the data is written.\r\n");

  // Close file
  //fresult = f_close(&fil);
  /*
  // Open file to read
  fresult = f_open(&fil, logs_file_name, FA_READ);

  // Read string from the file
  f_gets(buffer, sizeof(buffer), &fil);
  send_uart(buffer);

  // Close file
  f_close(&fil);
  bufclear();
  */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  LB_Humidifier(&bme280_sens_data, &USM_Humidifier, &Membrane_parameters);
	  LB_Data_Logging_Function(logs_file_name, &today, &time, &bme280_sens_dev, &bme280_sens_logging_data, &logging_counter, logging_index);
	  LB_Transition_Table[FSM_State][FSM_Event]();
	  LB_UI_Joystick_State_Refresh(&Joystick_Moved);	// TODO: fix bouncing
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (Joystick_button_Pin == GPIO_Pin)
	{
		LB_UI_Joystick_Switch_Pressed();
		if (state_thp_screen == FSM_State)
		{
			if (SI == unit_system)
			{
				unit_system = Imperial;
			}
			else
			{
				unit_system = SI;
			}
		}
		FSM_Event = event_button_pressed;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if (TIM10 == htim->Instance)
	{
		if (USM_Humidifier.membrane_is_active)
		{
			USM_Humidifier.membrane_active_counter++;
		}
		else
		{
			USM_Humidifier.membrane_delay_counter++;
		}
		thp_screen_counter++;
		clock_screen_update = true;
		if (NON_DATE_N_TIME_SET_STATE(FSM_State))
		{
			if (NEW_DAY == LB_Times_Ticking(&time))
			{
				LB_Next_Day(&today);
			}
			if (state_set_data_logging_period != FSM_State)
			{
				logging_counter++;
			}
		}
	}
}

// SD CARD
void LB_Data_Logging_Function(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data, uint16_t * delay_in_seconds, Data_Logging_Period_e period)
{
	  if( logging_period[period] == (*delay_in_seconds) )
	  {
		  LB_update_logs(file_name, pdate, ptime, dev, data);
		  *delay_in_seconds = 0;
	  }
}

/*void send_uart (char * string)
{
	uint8_t len = strlen(string);
	HAL_UART_Transmit(&huart2, (uint8_t *) string, len, 2000); // transmit in blocking mode
}*/

/*int bufsize (char * buf)
{
	int i = 0;
	while ('\0' != *buf++)
	{
		i++;
	}
	return i;
}*/

/*void bufclear (void)
{
	for (int i = 0; i < MAX_SD_CARD_BUFF; i++)
	{
		buffer[i] = '\0';
	}
}*/

FRESULT LB_update_logs(char * file_name, const Date_t * pdate, const Time_t * ptime, struct bme280_dev * dev, struct bme280_data * data)
{
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
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
