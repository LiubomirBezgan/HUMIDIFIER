/**
  ******************************************************************************
  * @file    LIB_Config.h
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief     This file provides configurations for low layer hardware libraries.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USE_LIB_CONFIG_H_
#define _USE_LIB_CONFIG_H_
//Macro Definition

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
//#include "mxconstants.h"
#include "main.h"
#include "Fonts.h"

extern SPI_HandleTypeDef hspi3;

#define __SSD1331_RES_SET()     HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);
#define __SSD1331_RES_CLR()     HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);

#define __SSD1331_DC_SET()      HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
#define __SSD1331_DC_CLR()      HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);

#define __SSD1331_CS_SET()      HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
#define __SSD1331_CS_CLR()      HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);

// Transmisja danych przez SPI w trybie blokujacym
#define __SSD1331_WRITE_BYTE(__DATA) HAL_SPI_Transmit(&hspi3, &__DATA, 1, 1000)

// Transmisja danych przez SPI z wykorzystaniem przerwan
//#define __SSD1331_WRITE_BYTE(__DATA) HAL_SPI_Transmit_IT(&hspi3, &__DATA, 1)

// Transmisja danych przez SPI z wykorzystaniem DMA
//#define __SSD1331_WRITE_BYTE(__DATA) HAL_SPI_Transmit_DMA(&hspi3, &__DATA, 1)


/*------------------------------------------------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */


#endif

/*-------------------------------END OF FILE-------------------------------*/

