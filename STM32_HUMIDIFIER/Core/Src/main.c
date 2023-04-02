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
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// Humidity sensor
#include "bme280_add.h"
#include "bme280_defs.h"

// OLED
#include "SSD1331.h"

// Generic
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_LEN 50

// OLED
#define T_CENTER_X 3
#define T_CENTER_Y 10
#define H_CENTER_X 3
#define H_CENTER_Y 29
#define P_CENTER_X 3
#define P_CENTER_Y 48
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// Temperature, humidity and pressure measurement
struct bme280_dev bme280_sens_dev;
struct bme280_data bme280_sens_data;

// Dana output

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LB_send_mes_via_UART(char * string);
void LB_send_data_via_UART(const struct bme280_data * data);
void LB_ssd1331_print_data(const struct bme280_data * data);
void LB_ssd1331_reset_screen(const struct bme280_data * data);
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
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_I2C1_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
  // The initialization of Humidity sensor
  if (BME280_OK != BME280_init(&bme280_sens_dev))
  {
	  LB_send_mes_via_UART("Initialization failed!\r\n");
  }
  // The initialization of OLED
  ssd1331_init();
  ssd1331_clear_screen(BLACK);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  LB_ssd1331_reset_screen(&bme280_sens_data);
	  if (BME280_OK != BME280_read_data(&bme280_sens_dev, &bme280_sens_data))
	  {
		  LB_send_mes_via_UART("A failure to read a data!\r\n");
	  }
	  else
	  {
		  LB_ssd1331_print_data(&bme280_sens_data);
		  //LB_send_data_via_UART(&bme280_sens_data);
	  }
	  HAL_Delay(1000);
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
void LB_send_mes_via_UART(char * string)
{
	uint16_t message_length;
	uint8_t message[MAX_LEN];

	message_length = sprintf((char *) message, string);
	HAL_UART_Transmit_IT(&huart2, (uint8_t *) string, message_length);
}

void LB_send_data_via_UART(const struct bme280_data * data)
{
	uint16_t message_length;
	uint8_t message[MAX_LEN];


	message_length = sprintf((char *) message, "Temperature   Humidity   Pressure\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *) message, message_length, 100);
	message_length = sprintf((char *) message, "%.2f C       %.2f %%    %.2f P\r\n",data->temperature, data->humidity, data->pressure);
	HAL_UART_Transmit(&huart2, (uint8_t *) message, message_length, 100);
}

void LB_ssd1331_reset_screen(const struct bme280_data * data)
{
	uint8_t message[MAX_LEN];

	sprintf((char *) message, "T: %.2f C",data->temperature);
	ssd1331_display_string(T_CENTER_X, T_CENTER_Y, message, FONT_1608, BLACK);
	sprintf((char *) message, "H: %.2f %%",data->humidity);
	ssd1331_display_string(H_CENTER_X, H_CENTER_Y, message, FONT_1608, BLACK);
	sprintf((char *) message, "P: %.0f mmHg",data->pressure * 0.0075);
	ssd1331_display_string(P_CENTER_X, P_CENTER_Y, message, FONT_1608, BLACK);
}

void LB_ssd1331_print_data(const struct bme280_data * data)
{
	uint8_t message[MAX_LEN];

	sprintf((char *) message, "T: %.2f C",data->temperature);
	ssd1331_display_string(T_CENTER_X, T_CENTER_Y, message, FONT_1608, PURPLE);
	sprintf((char *) message, "H: %.2f %%",data->humidity);
	ssd1331_display_string(H_CENTER_X, H_CENTER_Y, message, FONT_1608, WHITE);
	sprintf((char *) message, "P: %.0f mmHg",data->pressure * 0.0075);
	ssd1331_display_string(P_CENTER_X, P_CENTER_Y, message, FONT_1608, YELLOW);

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
