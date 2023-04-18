/*
 * LB_USM_Humidifier.c
 *
 *  Created on: Apr 17, 2023
 *      Author: SKG.POTOP
 */

/* Includes ------------------------------------------------------------------*/
#include "LB_USM_Humidifier.h"

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  initializes a USM_Humidifier_settings_t data type (target humidity level,
  * 		the duration of membrane's active state and delay between consequent hydrations) with specified parameters
  * @param  p_USM_param points to the USM_Humidifier_settings_t data type (the parameters of humidifier)
  * @param  target_hum_lvl describes the target humidity level
  * @param  active_state_duration describes the duration of membrane's active state
  * @param  hum_delay describes the delay between consequent hydrations
  * @retval None
  */
void LB_Init_USM_Hum_Parameters(USM_Humidifier_settings_t * p_USM_param, Hum_Level_e target_hum_lvl, Hum_Duration_e active_state_duration, Hum_Delay_e hum_delay)
{
	p_USM_param->target_hum_level = target_hum_lvl;
	p_USM_param->active_state_duration = active_state_duration;
	p_USM_param->humidifier_delay = hum_delay;
}
