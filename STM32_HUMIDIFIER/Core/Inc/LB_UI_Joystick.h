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
#include <stm32f411xe.h>
#include "stm32f4xx_hal.h"

/* Program-specific declarations ---------------------------------------------*/
#define DIMENSION 2					// the number of coordinates
#define JOYSTICK_LOWER_LIMIT 1050
#define JOYSTICK_UPPER_LIMIT 3050
#define JOYSTICK_PRESS_DELAY 555
#define JOYSTICK_MOVE_DELAY 268
#define JOYSTICK_LEFT(X) ((X).axis[0] < JOYSTICK_LOWER_LIMIT)
#define JOYSTICK_RIGHT(X) ((X).axis[0] > JOYSTICK_UPPER_LIMIT)
#define JOYSTICK_UP(X) ((X).axis[1] > JOYSTICK_UPPER_LIMIT)
#define JOYSTICK_DOWN(X) ((X).axis[1] < JOYSTICK_LOWER_LIMIT)

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
  * @retval HAL status
  */
HAL_StatusTypeDef LB_ADC_Start_DMA(ADC_HandleTypeDef* hadc, Joystick_t * pJoystick);

/**
  * @brief  initializes a Button_t data type to false
  * @param  pButton points to the Button_t data type
  * @retval None
  */
void LB_Init_Button(Button_t * pButton);

/**
  * @brief  enables EXTI interrupts on Line 4 and sets a Button_t data types (movement and press) to false (motionless and not pressed)
  * @param  pButton_Moved points to the Button_t data type, which describes the movement state of joystick
  * @param  pButton_Pressed points to the Button_t data type, which describes the press state of joystick
  * @retval None
  */
void LB_UI_Joystick_State_Refresh(Button_t * pButton_Moved, Button_t * pButton_Pressed);

/**
  * @brief  desables EXTI interrupts on Line 4
  * @param  pButton points to the Button_t data type
  * @retval None
  */
void LB_UI_Joystick_Switch_Pressed(Button_t * pButton);
#endif /* INC_LB_UI_JOYSTICK_H_ */
