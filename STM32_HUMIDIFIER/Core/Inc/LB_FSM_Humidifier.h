/*
 * LB_FSM_Humidifier.h
 *
 *  Created on: 5 apr. 2023
 *      Author: SKG.POTOP
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_LB_FSM_HUMIDIFIER_H_
#define INC_LB_FSM_HUMIDIFIER_H_

/* Includes ------------------------------------------------------------------*/
// Date and time
#include "LB_date.h"
#include "LB_time.h"
#include "LB_date&time_add.h"

// Humidity sensor
#include "bme280_defs.h"
#include "bme280_add.h"

// UI
#include "LB_UI_Joystick.h"

// OLED
#include "SSD1331.h"

// USM Humidifier
#include "LB_USM_Humidifier.h"

// Generic
#include <stdint.h>
#include <stdio.h>

/* Program-specific declarations ---------------------------------------------*/
#define MAX_LEN_DATA 12
#define REFRESH_FREQ 5
#define NON_DATE_N_TIME_SET_STATE(X) ( ((X) > state_set_date_d) || ((X) < state_set_time_h))
#define DATA_LOGGING_COLOR_TEXT WHITE
#define DATA_LOGGING_COLOR_DIGITS RED

/* General type definitions --------------------------------------------------*/
typedef enum {
	state_thp_screen,
	state_humidifier_screen,
	state_clock_screen,
	state_data_logging_period_screen,
	state_set_humidity,
	state_set_duration,
	state_set_delay,
	state_set_time_h,
	state_set_time_m,
	state_set_time_s,
	state_set_date_y,
	state_set_date_m,
	state_set_date_d,
	state_set_data_logging_period,
	STATE_MAX
} STATE_e;

typedef enum {
	event_none,
	event_button_pressed,
	event_joystick_right,
	event_joystick_left,
	EVENT_MAX
} EVENT_e;

typedef void (*TRANSITION_FUNC_PTR_t)(void);

// SD CARD
typedef enum {
	logging_1_min,
	logging_5_min,
	logging_10_min,
	logging_15_min,
	logging_20_min,
	logging_30_min,
	PERIOD_MAX
} Data_Logging_Period_e;

/* Function prototypes -------------------------------------------------------*/
/**
  * @brief  prints the data screen that shows temperature, humidity and pressure
  * @param  None
  * @retval None
  */
void thp_screen(void);

/**
  * @brief  prints the humidifier parameters screen that shows target humidity level, the duration of membrane active state and delay between consequent hydrations
  * @param  None
  * @retval None
  */
void humidifier_screen(void);

/**
  * @brief  prints the clock screen
  * @param  None
  * @retval None
  */
void clock_screen(void);

/**
  * @brief  prints the data logging period screen
  * @param  None
  * @retval None
  */
void data_logging_period_screen(void);

/**
  * @brief  sets the target humidity level and prints the corresponding screen
  * @param  None
  * @retval None
  */
void set_humidity(void);

/**
  * @brief  sets the duration of membrane active state and prints the corresponding screen
  * @param  None
  * @retval None
  */
void set_duration(void);

/**
  * @brief  sets the delay between consequent hydrations and prints the corresponding screen
  * @param  None
  * @retval None
  */
void set_delay(void);

/**
  * @brief  sets hours
  * @param  None
  * @retval None
  */
void set_time_h(void);

/**
  * @brief  sets minutes
  * @param  None
  * @retval None
  */
void set_time_m(void);

/**
  * @brief  sets seconds
  * @param  None
  * @retval None
  */
void set_time_s(void);

/**
  * @brief  sets a year
  * @param  None
  * @retval None
  */
void set_date_y(void);

/**
  * @brief  sets a month
  * @param  None
  * @retval None
  */
void set_date_m(void);

/**
  * @brief  sets a day
  * @param  None
  * @retval None
  */
void set_date_d(void);

/**
  * @brief  sets a data logging period
  * @param  None
  * @retval None
  */
void set_data_logging_period(void);

/** TODO: write the description
  * @brief  sets EVENT_e according to a joystick state
  * @param  None
  * @retval None
  */
void LB_DMA_Joystick_Event(const Joystick_t * p_Joystick, EVENT_e * p_event);

/** TODO: write the description
  * @brief  sets EVENT_e according to a joystick state
  * @param  None
  * @retval None
  */
void LB_Humidifier(const struct bme280_data * data, Membrane_t * p_membrane, const USM_Humidifier_settings_t * p_USM_Hum_settings);

/** TODO: write the description
  * @brief  sets EVENT_e according to a joystick state
  * @param  None
  * @retval None
  */
void LB_ssd1331_print_data_SI(const struct bme280_data * data);

/** TODO: write the description
  * @brief  sets EVENT_e according to a joystick state
  * @param  None
  * @retval None
  */
void LB_ssd1331_print_data_Imperial(const struct bme280_data * data);

/** TODO: write the description
  * @brief  sets EVENT_e according to a joystick state
  * @param  None
  * @retval None
  */
void LB_ssd1331_reset_screen_SI(const struct bme280_data * data);

/** TODO: write the description
  * @brief  sets EVENT_e according to a joystick state
  * @param  None
  * @retval None
  */
void LB_ssd1331_reset_screen_Imperial(const struct bme280_data * data);

#endif /* INC_LB_FSM_HUMIDIFIER_H_ */
