/*
 * LB_UI_Joystick.c
 *
 *  Created on: Apr 3, 2023
 *      Author: SKG.POTOP
 */

/* Includes ------------------------------------------------------------------*/
#include <LB_UI_Joystick.h>

/* Function definitions ------------------------------------------------------*/

/**
  * @brief  Enables ADC DMA request after last transfer (Single-ADC mode) and enables ADC peripheral for Joystick_t ADT
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @param  pJoystick The destination Buffer address.
  * @param  Length The length of data to be transferred from ADC peripheral to memory.
  * @retval HAL status
  */
HAL_StatusTypeDef LB_ADC_Start_DMA(ADC_HandleTypeDef* hadc, Joystick_t * pJoystick)
{
	return HAL_ADC_Start_DMA(hadc, (uint32_t*) pJoystick->axis, DIMENSION);
}

/**
  * @brief  initializes a Button_t data type to false
  * @param  pButton points to the Button_t data type
  * @retval None
  */
void LB_Init_Button(Button_t * pButton)
{
	*pButton = false;
}

/**
  * @brief  enables EXTI interrupts on Line 4 and sets a Button_t data type to false (motionless or not pressed)
  * @param  pButton points to the Button_t data type
  * @retval None
  */
void LB_UI_Joystick_State_Refresh(Button_t * pButton)
{
	  HAL_Delay(JOYSTICK_DELAY);				// Switch debouncing
	  EXTI->IMR |= ((uint32_t) EXTI_IMR_MR4);	// Interrupt request from line 4 is not masked TODO: maybe other place?
	  *pButton = false;
}

/**
  * @brief  desables EXTI interrupts on Line 4
  * @param  none
  * @retval None
  */
void LB_UI_Joystick_Switch_Pressed(void)
{
	EXTI->IMR &= ~((uint32_t) EXTI_IMR_MR4);	// Interrupt request from line 4 is masked
}
