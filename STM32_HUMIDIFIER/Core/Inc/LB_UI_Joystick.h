/*
 * LB_UI_Joystick.h
 *
 *  Created on: Apr 3, 2023
 *      Author: SKG.POTOP
 */

#ifndef INC_LB_UI_JOYSTICK_H_
#define INC_LB_UI_JOYSTICK_H_

/* Includes ------------------------------------------------------------------*/
#include <adc.h>
#include <stdbool.h>
#include <stm32f4xx_hal_def.h>

/* Program-specific declarations ---------------------------------------------*/
#define DIMENSION 2					// the number of coordinates
#define JOYSTICK_LOWER_LIMIT 1050
#define JOYSTICK_UPPER_LIMIT 3050
#define JOYSTICK_DELAY 268
#define JOYSTICK_LEFT(X) (X.axis[0] < JOYSTICK_LOWER_LIMIT)
#define JOYSTICK_RIGHT(X) (X.axis[0] > JOYSTICK_UPPER_LIMIT)
#define JOYSTICK_UP(X) (X.axis[1] > JOYSTICK_UPPER_LIMIT)
#define JOYSTICK_DOWN(X) (X.axis[1] < JOYSTICK_LOWER_LIMIT)

/* General type definitions --------------------------------------------------*/
/* redefine Joystick_t as appropriate */
typedef struct joystick {
	uint16_t axis[DIMENSION];               // 0 - X, 1 - Y
} Joystick_t;

typedef volatile bool Button_t;
/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  Enables ADC DMA request after last transfer (Single-ADC mode) and enables ADC peripheral for Joystick_t ADT
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @param  pJoystick The destination Buffer address.
  * @param  Length The length of data to be transferred from ADC peripheral to memory.
  * @retval HAL status
  */
HAL_StatusTypeDef LB_ADC_Start_DMA(ADC_HandleTypeDef* hadc, Joystick_t * pJoystick);

/**
  * @brief  initializes a Button_t data type to false
  * @param  pButton points to the Button_t data type
  * @retval None
  */
void LB_Init_Button(Button_t * pButton);
#endif /* INC_LB_UI_JOYSTICK_H_ */
