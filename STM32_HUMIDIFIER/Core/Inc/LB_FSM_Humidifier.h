/*
 * LB_FSM_Humidifier.h
 *
 *  Created on: Apr 5, 2023
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

/* Function prototypes -------------------------------------------------------*/
/**
  * @brief  initializes a finite-state machine
  * @param  pstate points to a variable that describes the state of FSM
  * @param  pevent points to a variable that describes the event of FSM
  * @retval None
  */
void LB_Init_FSM(STATE_e * pstate, EVENT_e * pevent);

/**
  * @brief  prints the data screen that shows temperature, humidity and pressure
  * @param  None
  * @retval None
  */
void thp_screen(void);

/**
  * @brief  prints the humidifier parameters screen that shows target humidity level, the duration of membrane active state and delay between consequent humidification processes
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
  * @brief  sets the delay between consequent humidification processes and prints the corresponding screen
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

/**
  * @brief  sets an EVENT_e variable according to a joystick state
  * @param  p_Joystick points to Joystick_t data type that describes current joystick state
  * @param  p_event points to variable that describes last event
  * @retval None
  */
void LB_DMA_Joystick_Event(const Joystick_t * p_Joystick, EVENT_e * p_event);

/**
  * @brief  controls the humidity of the air on the basis of current sensor data
  * @param  data points to bme280 sensor structure which comprises of temperature, pressure and humidity data
  * @param  p_membrane points to ultrasonic membrane structure which comprises of humidification parameters
  * @param  p_USM_Hum_settings points to humidifier settings structure
  * @retval None
  */
void LB_Humidifier(const struct bme280_data * data, Membrane_t * p_membrane, const USM_Humidifier_settings_t * p_USM_Hum_settings);

/**
  * @brief  resets temperature, humidity and pressure screen in the case of SI units
  * @param  data points to bme280 sensor structure which comprises of temperature, pressure and humidity data
  * @retval None
  */
void LB_ssd1331_print_data_SI(const struct bme280_data * data);

/**
  * @brief  resets temperature, humidity and pressure screen in the case of imperial units
  * @param  data points to bme280 sensor structure which comprises of temperature, pressure and humidity data
  * @retval None
  */
void LB_ssd1331_print_data_Imperial(const struct bme280_data * data);

/**
  * @brief  prints temperature, humidity and pressure screen in the case of SI units
  * @param  data points to bme280 sensor structure which comprises of temperature, pressure and humidity data
  * @retval None
  */
void LB_ssd1331_reset_screen_SI(const struct bme280_data * data);

/**
  * @brief  prints temperature, humidity and pressure screen in the case of imperial units
  * @param  data points to bme280 sensor structure which comprises of temperature, pressure and humidity data
  * @retval None
  */
void LB_ssd1331_reset_screen_Imperial(const struct bme280_data * data);

#endif /* INC_LB_FSM_HUMIDIFIER_H_ */
