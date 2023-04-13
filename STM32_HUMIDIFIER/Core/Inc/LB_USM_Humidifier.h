/*
 * LB_USM_Humidifier.h
 *
 *  Created on: 11 apr 2023
 *      Author: SKG.POTOP
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_LB_USM_HUMIDIFIER_H_
#define INC_LB_USM_HUMIDIFIER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Program-specific declarations ---------------------------------------------*/
// Ultrasonic membrane humidifier
#define MEMBRANE_DURATION 60		// 1 min
#define MEMBRANE_DELAY 120			// 2 min
#define HUMIDITY_LEVEL 50.0			// [%]

// OLED
#define HUM_LVL_COLOR LIGHT_SEA_GREEN
#define HUM_DURATION_COLOR DARK_ORANGE
#define HUM_DELAY_COLOR MEDIUM_VIOLET_RED

#define HUM_LVL_CENTER_X 1
#define HUM_LVL_CENTER_Y 7
#define HUM_DURATION_CENTER_X HUM_LVL_CENTER_X
#define HUM_DURATION_CENTER_Y (HUM_LVL_CENTER_Y + 18)
#define HUM_DELAY_CENTER_X HUM_LVL_CENTER_X
#define HUM_DELAY_CENTER_Y (HUM_DURATION_CENTER_Y + 18)

#define LINE_OFFSET_1206_Y 12
#define LINE_LENGTH_HUM_LVL_X 77
#define LINE_LENGTH_HUM_DURATION_X 89
#define LINE_LENGTH_HUM_DELAY_X 89

#define HUM_LVL_35 35			// [%]
#define HUM_LVL_40 40			// [%]
#define HUM_LVL_45 45			// [%]
#define HUM_LVL_50 50			// [%]
#define HUM_LVL_55 55			// [%]
#define HUM_LVL_60 60			// [%]

#define HUM_DURATION_05 30		// [s]
#define HUM_DURATION_10 60		// [s]
#define HUM_DURATION_15 90		// [s]

#define HUM_DELAY_1 60			// [s]
#define HUM_DELAY_2 120		// [s]
#define HUM_DELAY_5 300		// [s]
#define HUM_DELAY_10 600		// [s]
#define HUM_DELAY_15 900		// [s]


/* General type definitions --------------------------------------------------*/
typedef struct membrane {
	uint16_t membrane_active_counter;
	uint16_t membrane_delay_counter;
	bool membrane_is_active;
} Membrane_t;

typedef enum {
	hum_lvl_35,				// [%]
	hum_lvl_40,				// [%]
	hum_lvl_45,				// [%]
	hum_lvl_50,				// [%]
	hum_lvl_55,				// [%]
	hum_lvl_60,				// [%]
	HUM_LVL_MAX
} Hum_Level_e;

typedef enum {
	hum_duration_05,		// 30 s
	hum_duration_10,		// 60 s
	hum_duration_15,		// 90 s
	HUM_DURATION_MAX
} Hum_Duration_e;

typedef enum {
	hum_delay_1,			// 60 s
	hum_delay_2,			// 120 s
	hum_delay_5,			// 300 s
	hum_delay_10,			// 600 s
	hum_delay_15,			// 900 s
	HUM_DELAY_MAX
} Hum_Delay_e;

/* redefine USM_Humidifier_t as appropriate */
typedef struct usm_humidifier {
	Hum_Level_e target_hum_level;			// [%]
	Hum_Duration_e active_state_duration;	// [s]
	Hum_Delay_e humidifier_delay;			// [s]
} USM_Humidifier_settings_t;

/* Function prototypes -------------------------------------------------------*/
/**
  * @brief  initializes a Date_t data type (date) and set the date of the library's latest update
  * @param  pdate points to the Date_t data type (date)
  * @retval None
  */
void LB_Init_USM_Hum_Parameters(USM_Humidifier_settings_t * p_USM_param);
#endif /* INC_LB_USM_HUMIDIFIER_H_ */
