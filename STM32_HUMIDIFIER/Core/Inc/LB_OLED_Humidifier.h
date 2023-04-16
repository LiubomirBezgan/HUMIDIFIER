/*
 * LB_OLED_Humidifier.h
 *
 *  Created on: 13 apr 2023
 *      Author: SKG.POTOP
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_LB_OLED_HUMIDIFIER_H_
#define INC_LB_OLED_HUMIDIFIER_H_

/* Includes ------------------------------------------------------------------*/
#include "SSD1331.h"

/* Program-specific declarations ---------------------------------------------*/
// General
#define BACKGROUND_COLOR BLACK

// Temperature, humidity and pressure screen
#define T_CENTER_X 3
#define T_CENTER_Y 5
#define H_CENTER_X T_CENTER_X
#define H_CENTER_Y (T_CENTER_Y + 19)
#define P_CENTER_X T_CENTER_X
#define P_CENTER_Y (H_CENTER_Y + 19)
#define T_COLOR PURPLE
#define H_COLOR WHITE
#define P_COLOR YELLOW

#endif /* INC_LB_OLED_HUMIDIFIER_H_ */
