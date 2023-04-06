/*
 * LB_FSM_Humidifier.c
 *
 *  Created on: 5 apr. 2023
 *      Author: SKG.POTOP
 */

/* Includes ------------------------------------------------------------------*/
// Main
#include <LB_FSM_Humidifier.h>

// OLED
#include "SSD1331.h"

#include "bme280_add.h"

// Date and time
#include "LB_date&time_add.h"

// UI
#include "LB_UI_Joystick.h"

// Generic
#include <stdio.h>

#define REFRESH_FREQ 5

// Date and time
extern Time_t time;
extern uint8_t message_time[9];
extern Date_t today;
extern uint8_t message_date[12];

// UI
extern Joystick_t Joystick;

// Main
extern STATE_e FSM_State;
extern EVENT_e FSM_Event;

// Temperature, humidity and pressure measurement
extern struct bme280_dev bme280_sens_dev;
extern struct bme280_data bme280_sens_data;
extern UNITS_e unit_system;

// OLED
extern void LB_ssd1331_print_data_SI(const struct bme280_data * data);
extern void LB_ssd1331_print_data_Imperial(const struct bme280_data * data);
extern void LB_ssd1331_reset_screen_SI(const struct bme280_data * data);
extern void LB_ssd1331_reset_screen_Imperial(const struct bme280_data * data);
extern uint8_t thp_screen_counter;
extern volatile bool clock_screen_update;

// SD CARD
extern uint16_t logging_frequency;

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  prints the data screen that shows temperature, humidity and pressure
  * @param  None
  * @retval None
  */
void thp_screen(void)
{
	HAL_Delay(JOYSTICK_DELAY);
	FSM_State = state_thp_screen;
	if (event_none != FSM_Event)
	{
		ssd1331_clear_screen(BLACK);
		BME280_read_data(&bme280_sens_dev, &bme280_sens_data); // TODO: add bme280_sens_data_prev to reduce the delay of screen refresh
		if (SI == unit_system)
		{
			LB_ssd1331_print_data_SI(&bme280_sens_data);
		}
		else
		{
			LB_ssd1331_print_data_Imperial(&bme280_sens_data);
		}
		thp_screen_counter = 0;
	}
	else
	{
		if (thp_screen_counter > REFRESH_FREQ)
		{
			thp_screen_counter = 0;
			if (SI == unit_system)
			{
				LB_ssd1331_reset_screen_SI(&bme280_sens_data);
				BME280_read_data(&bme280_sens_dev, &bme280_sens_data); // TODO: add bme280_sens_data_prev to reduce the delay of screen refresh
				LB_ssd1331_print_data_SI(&bme280_sens_data);
			}
			else
			{
				LB_ssd1331_reset_screen_Imperial(&bme280_sens_data);
				BME280_read_data(&bme280_sens_dev, &bme280_sens_data); // TODO: add bme280_sens_data_prev to reduce the delay of screen refresh
				LB_ssd1331_print_data_Imperial(&bme280_sens_data);
			}
		}

	}
	FSM_Event = event_none;
}

/**
  * @brief  prints the clock screen
  * @param  None
  * @retval None
  */
void clock_screen(void)
{
	HAL_Delay(JOYSTICK_DELAY);
	FSM_State = state_clock_screen;
	if (event_none != FSM_Event)
	{
		ssd1331_clear_screen(BLACK);
		LB_Date_to_Str(&today, (char *) message_date);
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, COLOR_DATE);
		LB_Time_to_Str(&time, (char *) message_time);
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, COLOR_TIME);
	}
	else
	{
		if (clock_screen_update)
		{
			ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, BLACK);
			LB_Date_to_Str(&today, (char *) message_date);
			ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, COLOR_DATE);

			ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, BLACK);
			LB_Time_to_Str(&time, (char *) message_time);
			ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, COLOR_TIME);

			clock_screen_update = false;
		}
	}
	FSM_Event = event_none;
}

/**
  * @brief  prints the data logging period screen
  * @param  None
  * @retval None
  */
void data_logging_period_screen(void)
{
	uint8_t message_logging_period[13];

	HAL_Delay(JOYSTICK_DELAY);
	FSM_State = state_data_logging_period_screen;
	if (event_none != FSM_Event)
	{
		ssd1331_clear_screen(BLACK);
	}
	sprintf( (char *) message_logging_period, "DATA LOGGING");
	ssd1331_display_string(LOGGING_CENTER_X, LOGGING_CENTER_Y, message_logging_period, FONT_1608, COLOR_TIME);
	sprintf( (char *) message_logging_period, "  FREQUENCY:");
	ssd1331_display_string(LOGGING_CENTER_X, LOGGING_CENTER_Y, message_logging_period, FONT_1608, COLOR_TIME);
	sprintf( (char *) message_logging_period, "      %2d min", (logging_frequency / 60));
	ssd1331_display_string(LOGGING_CENTER_X, LOGGING_CENTER_Y, message_logging_period, FONT_1608, COLOR_TIME);
	FSM_Event = event_none;
}

/**
  * @brief  sets hours
  * @param  None
  * @retval None
  */
void set_time_h(void)
{

}

/**
  * @brief  sets minutes
  * @param  None
  * @retval None
  */
void set_time_m(void)
{

}

/**
  * @brief  sets seconds
  * @param  None
  * @retval None
  */
void set_time_s(void)
{

}

/**
  * @brief  sets a year
  * @param  None
  * @retval None
  */
void set_date_y(void)
{

}

/**
  * @brief  sets a month
  * @param  None
  * @retval None
  */
void set_date_m(void)
{

}

/**
  * @brief  sets a day
  * @param  None
  * @retval None
  */
void set_date_d(void)
{

}

/**
  * @brief  sets a data logging period
  * @param  None
  * @retval None
  */
void set_data_logging_period(void)
{

}
