/*
 * LB_FSM_Humidifier.c
 *
 *  Created on: 5 apr. 2023
 *      Author: SKG.POTOP
 */

/* Includes ------------------------------------------------------------------*/
// Main
#include <LB_FSM_Humidifier.h>

// Date and time
extern Time_t time;
extern uint8_t message_time[9];
extern Date_t today;
extern uint8_t message_date[12];

// UI
extern Joystick_t Joystick;
extern Button_t Joystick_Moved;

// Main
extern STATE_e FSM_State;
extern EVENT_e FSM_Event;

// Temperature, humidity and pressure measurement
extern struct bme280_dev bme280_sens_dev;
extern struct bme280_data bme280_sens_data;
extern struct bme280_data bme280_sens_data_prev; // TODO: check it
extern UNITS_e unit_system;

// OLED
extern void LB_ssd1331_print_data_SI(const struct bme280_data * data);
extern void LB_ssd1331_print_data_Imperial(const struct bme280_data * data);
extern void LB_ssd1331_reset_screen_SI(const struct bme280_data * data);
extern void LB_ssd1331_reset_screen_Imperial(const struct bme280_data * data);
extern uint8_t thp_screen_counter;
extern volatile bool clock_screen_update;

// SD CARD
extern uint16_t logging_period[];
extern Data_Logging_Period_e logging_index;

// USM Humidifier
extern USM_Humidifier_settings_t Membrane_parameters;
extern Membrane_t USM_Humidifier;
extern const uint8_t Hum_Level[];
extern const uint16_t Hum_Duration[];
extern const uint16_t Hum_Delay[];

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  prints the data screen that shows temperature, humidity and pressure
  * @param  None
  * @retval None
  */
void thp_screen(void)
{
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
		if (REFRESH_FREQ <= thp_screen_counter)
		{
			thp_screen_counter = 0;
			if (SI == unit_system)
			{
				/*LB_ssd1331_reset_screen_SI(&bme280_sens_data);
				BME280_read_data(&bme280_sens_dev, &bme280_sens_data); // TODO: add bme280_sens_data_prev to reduce the delay of screen refresh
				LB_ssd1331_print_data_SI(&bme280_sens_data);*/
				bme280_sens_data_prev = bme280_sens_data;
				BME280_read_data(&bme280_sens_dev, &bme280_sens_data); // TODO: add bme280_sens_data_prev to reduce the delay of screen refresh
				LB_ssd1331_reset_screen_SI(&bme280_sens_data_prev);
				LB_ssd1331_print_data_SI(&bme280_sens_data);
			}
			else
			{
				LB_ssd1331_reset_screen_Imperial(&bme280_sens_data);
				BME280_read_data(&bme280_sens_dev, &bme280_sens_data); // TODO: add bme280_sens_data_prev to reduce the delay of screen refresh
				LB_ssd1331_print_data_Imperial(&bme280_sens_data);
				/*bme280_sens_data_prev = bme280_sens_data;
				BME280_read_data(&bme280_sens_dev, &bme280_sens_data); // TODO: add bme280_sens_data_prev to reduce the delay of screen refresh
				LB_ssd1331_reset_screen_Imperial(&bme280_sens_data_prev);
				LB_ssd1331_print_data_Imperial(&bme280_sens_data);*/
			}
		}

	}
	FSM_Event = event_none;
}

/**
  * @brief  prints the humidifier parameters screen that shows target humidity level, the duration of membrane active state and delay between consequent hydrations
  * @param  None
  * @retval None
  */
void humidifier_screen(void)
{
	uint8_t message_humidifier_screen[17];

	FSM_State = state_humidifier_screen;
	if (event_none != FSM_Event)
	{
		ssd1331_clear_screen(BLACK);
	}

	sprintf( (char *) message_humidifier_screen, "HUM LVL: %2u %%", Hum_Level[Membrane_parameters.target_hum_level]);
	ssd1331_display_string(HUM_LVL_CENTER_X, HUM_LVL_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_LVL_COLOR);
	sprintf( (char *) message_humidifier_screen, "ACTIVE: %.1f min", ((float) Hum_Duration[Membrane_parameters.active_state_duration] / 60) );
	ssd1331_display_string(HUM_DURATION_CENTER_X, HUM_DURATION_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_DURATION_COLOR);
	sprintf( (char *) message_humidifier_screen, "DELAY:   %2u min", (Hum_Delay[Membrane_parameters.humidifier_delay] / 60));
	ssd1331_display_string(HUM_DELAY_CENTER_X, HUM_DELAY_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_DELAY_COLOR);

	FSM_Event = event_none;
}

/**
  * @brief  prints the clock screen
  * @param  None
  * @retval None
  */
void clock_screen(void)
{
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
		ssd1331_draw_line(DATE_OFFSET_X, D_LINE_OFFSET_Y, END_OF_THE_LINE_DATE, D_LINE_OFFSET_Y, BLACK);
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
	uint8_t message_logging_period[11];

	FSM_State = state_data_logging_period_screen;

	if (event_none != FSM_Event)
	{
		ssd1331_clear_screen(BLACK);
	}
	sprintf( (char *) message_logging_period, "  LOGGING");
	ssd1331_display_string(DATA_LOGGING_CENTER_X, DATA_LOGGING_CENTER_Y, message_logging_period, FONT_1608, DATA_LOGGING_COLOR_TEXT);
	sprintf( (char *) message_logging_period, "  PERIOD:");
	ssd1331_display_string(PERIOD_CENTER_X, PERIOD_CENTER_Y, message_logging_period, FONT_1608, DATA_LOGGING_COLOR_TEXT);
	sprintf( (char *) message_logging_period, "   %2u min", (logging_period[logging_index] / 60));
	ssd1331_display_string(LOG_MIN_CENTER_X, LOG_MIN_CENTER_Y, message_logging_period, FONT_1608, DATA_LOGGING_COLOR_DIGITS);

	FSM_Event = event_none;
}

/**
  * @brief  sets the target humidity level and prints the corresponding screen
  * @param  None
  * @retval None
  */
void set_humidity(void)
{
	uint8_t message_humidifier_screen[17];

	FSM_State = state_set_humidity;
	ssd1331_draw_line(HUM_LVL_CENTER_X, HUM_LVL_CENTER_Y + LINE_OFFSET_1206_Y, HUM_LVL_CENTER_X + LINE_LENGTH_HUM_LVL_X, HUM_LVL_CENTER_Y + LINE_OFFSET_1206_Y, HUM_LVL_COLOR);
	if (JOYSTICK_UP(Joystick))
	{
		sprintf( (char *) message_humidifier_screen, "HUM LVL: %2u %%", Hum_Level[Membrane_parameters.target_hum_level]);
		ssd1331_display_string(HUM_LVL_CENTER_X, HUM_LVL_CENTER_Y, message_humidifier_screen, FONT_1206, BLACK);
		if (++(Membrane_parameters.target_hum_level) >= HUM_LVL_MAX)
		{
			Membrane_parameters.target_hum_level = 0;
		}
		sprintf( (char *) message_humidifier_screen, "HUM LVL: %2u %%", Hum_Level[Membrane_parameters.target_hum_level]);
		ssd1331_display_string(HUM_LVL_CENTER_X, HUM_LVL_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_LVL_COLOR);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		sprintf( (char *) message_humidifier_screen, "HUM LVL: %2u %%", Hum_Level[Membrane_parameters.target_hum_level]);
		ssd1331_display_string(HUM_LVL_CENTER_X, HUM_LVL_CENTER_Y, message_humidifier_screen, FONT_1206, BLACK);
		if (0 == (Membrane_parameters.target_hum_level)--)
		{
			Membrane_parameters.target_hum_level = HUM_LVL_MAX - 1;
		}
		sprintf( (char *) message_humidifier_screen, "HUM LVL: %2u %%", Hum_Level[Membrane_parameters.target_hum_level]);
		ssd1331_display_string(HUM_LVL_CENTER_X, HUM_LVL_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_LVL_COLOR);
	}
	FSM_Event = event_none;
}

/**
  * @brief  sets the duration of membrane active state and prints the corresponding screen
  * @param  None
  * @retval None
  */
void set_duration(void)
{
	uint8_t message_humidifier_screen[17];

	FSM_State = state_set_duration;
	ssd1331_draw_line(HUM_LVL_CENTER_X, HUM_LVL_CENTER_Y + LINE_OFFSET_1206_Y, HUM_LVL_CENTER_X + LINE_LENGTH_HUM_LVL_X, HUM_LVL_CENTER_Y + LINE_OFFSET_1206_Y, BLACK);
	ssd1331_draw_line(HUM_DURATION_CENTER_X, HUM_DURATION_CENTER_Y + LINE_OFFSET_1206_Y, HUM_DURATION_CENTER_X + LINE_LENGTH_HUM_DURATION_X, HUM_DURATION_CENTER_Y + LINE_OFFSET_1206_Y, HUM_DURATION_COLOR);
	if (JOYSTICK_UP(Joystick))
	{
		sprintf( (char *) message_humidifier_screen, "ACTIVE: %.1f min", ((float) Hum_Duration[Membrane_parameters.active_state_duration] / 60) );
		ssd1331_display_string(HUM_DURATION_CENTER_X, HUM_DURATION_CENTER_Y, message_humidifier_screen, FONT_1206, BLACK);
		if (++(Membrane_parameters.active_state_duration) >= HUM_DURATION_MAX)
		{
			Membrane_parameters.active_state_duration = 0;
		}
		sprintf( (char *) message_humidifier_screen, "ACTIVE: %.1f min", ((float) Hum_Duration[Membrane_parameters.active_state_duration] / 60) );
		ssd1331_display_string(HUM_DURATION_CENTER_X, HUM_DURATION_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_DURATION_COLOR);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		sprintf( (char *) message_humidifier_screen, "ACTIVE: %.1f min", ((float) Hum_Duration[Membrane_parameters.active_state_duration] / 60) );
		ssd1331_display_string(HUM_DURATION_CENTER_X, HUM_DURATION_CENTER_Y, message_humidifier_screen, FONT_1206, BLACK);
		if (0 == (Membrane_parameters.active_state_duration)--)
		{
			Membrane_parameters.active_state_duration = HUM_DURATION_MAX - 1;
		}
		sprintf( (char *) message_humidifier_screen, "ACTIVE: %.1f min", ((float) Hum_Duration[Membrane_parameters.active_state_duration] / 60) );
		ssd1331_display_string(HUM_DURATION_CENTER_X, HUM_DURATION_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_DURATION_COLOR);
	}
	FSM_Event = event_none;
}

/**
  * @brief  sets the delay between consequent hydrations and prints the corresponding screen
  * @param  None
  * @retval None
  */
void set_delay(void)
{
	uint8_t message_humidifier_screen[17];

	FSM_State = state_set_delay;
	ssd1331_draw_line(HUM_DURATION_CENTER_X, HUM_DURATION_CENTER_Y + LINE_OFFSET_1206_Y, HUM_DURATION_CENTER_X + LINE_LENGTH_HUM_DURATION_X, HUM_DURATION_CENTER_Y + LINE_OFFSET_1206_Y, BLACK);
	ssd1331_draw_line(HUM_DELAY_CENTER_X, HUM_DELAY_CENTER_Y + LINE_OFFSET_1206_Y, HUM_DELAY_CENTER_X + LINE_LENGTH_HUM_DELAY_X, HUM_DELAY_CENTER_Y + LINE_OFFSET_1206_Y, HUM_DELAY_COLOR);
	if (JOYSTICK_UP(Joystick))
	{
		sprintf( (char *) message_humidifier_screen, "DELAY:   %2u min", (Hum_Delay[Membrane_parameters.humidifier_delay] / 60));
		ssd1331_display_string(HUM_DELAY_CENTER_X, HUM_DELAY_CENTER_Y, message_humidifier_screen, FONT_1206, BLACK);
		if (++(Membrane_parameters.humidifier_delay) >= HUM_DELAY_MAX)
		{
			Membrane_parameters.humidifier_delay = 0;
		}
		sprintf( (char *) message_humidifier_screen, "DELAY:   %2u min", (Hum_Delay[Membrane_parameters.humidifier_delay] / 60));
		ssd1331_display_string(HUM_DELAY_CENTER_X, HUM_DELAY_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_DELAY_COLOR);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		sprintf( (char *) message_humidifier_screen, "DELAY:   %2u min", (Hum_Delay[Membrane_parameters.humidifier_delay] / 60));
		ssd1331_display_string(HUM_DELAY_CENTER_X, HUM_DELAY_CENTER_Y, message_humidifier_screen, FONT_1206, BLACK);
		if (0 == (Membrane_parameters.humidifier_delay)--)
		{
			Membrane_parameters.humidifier_delay = HUM_DELAY_MAX - 1;
		}
		sprintf( (char *) message_humidifier_screen, "DELAY:   %2u min", (Hum_Delay[Membrane_parameters.humidifier_delay] / 60));
		ssd1331_display_string(HUM_DELAY_CENTER_X, HUM_DELAY_CENTER_Y, message_humidifier_screen, FONT_1206, HUM_DELAY_COLOR);
	}
	FSM_Event = event_none;
}

/**
  * @brief  sets hours
  * @param  None
  * @retval None
  */
void set_time_h(void)
{
	FSM_State = state_set_time_h;

	if (event_joystick_left == FSM_Event)
	{
		ssd1331_draw_line(M_LINE_OFFSET_X, M_LINE_OFFSET_Y, M_LINE_OFFSET_X + LINE_LENGTH_X, M_LINE_OFFSET_Y, BLACK);
	}
	else if (event_joystick_right == FSM_Event)
	{
		ssd1331_draw_line(S_LINE_OFFSET_X, S_LINE_OFFSET_Y, S_LINE_OFFSET_X + LINE_LENGTH_X, S_LINE_OFFSET_Y, BLACK);
	}

	ssd1331_draw_line(H_LINE_OFFSET_X, H_LINE_OFFSET_Y, H_LINE_OFFSET_X + LINE_LENGTH_X, H_LINE_OFFSET_Y, COLOR_TIME);

	if (JOYSTICK_UP(Joystick))
	{
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, BLACK);
		LB_Set_Next_Hour(&time);
		LB_Time_to_Str(&time, (char *) message_time);
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, COLOR_TIME);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, BLACK);
		LB_Set_Prev_Hour(&time);
		LB_Time_to_Str(&time, (char *) message_time);
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, COLOR_TIME);
	}

	FSM_Event = event_none;
}

/**
  * @brief  sets minutes
  * @param  None
  * @retval None
  */
void set_time_m(void)
{
	FSM_State = state_set_time_m;

	if (event_joystick_left == FSM_Event)
	{
		ssd1331_draw_line(S_LINE_OFFSET_X, S_LINE_OFFSET_Y, S_LINE_OFFSET_X + LINE_LENGTH_X, S_LINE_OFFSET_Y, BLACK);
	}
	else if (event_joystick_right == FSM_Event)
	{
		ssd1331_draw_line(H_LINE_OFFSET_X, H_LINE_OFFSET_Y, H_LINE_OFFSET_X + LINE_LENGTH_X, H_LINE_OFFSET_Y, BLACK);
	}

	ssd1331_draw_line(M_LINE_OFFSET_X, M_LINE_OFFSET_Y, M_LINE_OFFSET_X + LINE_LENGTH_X, M_LINE_OFFSET_Y, COLOR_TIME);

	if (JOYSTICK_UP(Joystick))
	{
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, BLACK);
		LB_Set_Next_Minute(&time);
		LB_Time_to_Str(&time, (char *) message_time);
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, COLOR_TIME);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, BLACK);
		LB_Set_Prev_Minute(&time);
		LB_Time_to_Str(&time, (char *) message_time);
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, COLOR_TIME);
	}

	FSM_Event = event_none;
}

/**
  * @brief  sets seconds
  * @param  None
  * @retval None
  */
void set_time_s(void)
{
	FSM_State = state_set_time_s;

	if (event_joystick_left == FSM_Event)
	{
		ssd1331_draw_line(H_LINE_OFFSET_X, H_LINE_OFFSET_Y, H_LINE_OFFSET_X + LINE_LENGTH_X, H_LINE_OFFSET_Y, BLACK);
	}
	else if (event_joystick_right == FSM_Event)
	{
		ssd1331_draw_line(M_LINE_OFFSET_X, M_LINE_OFFSET_Y, M_LINE_OFFSET_X + LINE_LENGTH_X, M_LINE_OFFSET_Y, BLACK);
	}

	ssd1331_draw_line(S_LINE_OFFSET_X, S_LINE_OFFSET_Y, S_LINE_OFFSET_X + LINE_LENGTH_X, S_LINE_OFFSET_Y, COLOR_TIME);

	if (JOYSTICK_UP(Joystick))
	{
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, BLACK);
		LB_Set_Next_Second(&time);
		LB_Time_to_Str(&time, (char *) message_time);
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, COLOR_TIME);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, BLACK);
		LB_Set_Prev_Second(&time);
		LB_Time_to_Str(&time, (char *) message_time);
		ssd1331_display_string(CENTER_X, CENTER_Y, message_time, FONT_1608, COLOR_TIME);
	}

	FSM_Event = event_none;
}

/**
  * @brief  sets a year
  * @param  None
  * @retval None
  */
void set_date_y(void)
{
	FSM_State = state_set_date_y;

	if (event_joystick_left == FSM_Event)
	{
		ssd1331_draw_line(D_LINE_OFFSET_X, D_LINE_OFFSET_Y, D_LINE_OFFSET_X + LINE_LENGTH_DAY_X, D_LINE_OFFSET_Y, BLACK);
	}
	else if (event_joystick_right == FSM_Event)
	{
		ssd1331_draw_line(MONTH_LINE_OFFSET_X, MONTH_LINE_OFFSET_Y, MONTH_LINE_OFFSET_X + LINE_LENGTH_MONTH_X, MONTH_LINE_OFFSET_Y, BLACK);
	}

	ssd1331_draw_line(Y_LINE_OFFSET_X, Y_LINE_OFFSET_Y, Y_LINE_OFFSET_X + LINE_LENGTH_YEAR_X, Y_LINE_OFFSET_Y, COLOR_DATE);

	if (JOYSTICK_UP(Joystick))
	{
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, BLACK);
		LB_Next_Year(&today);
		LB_Date_to_Str(&today, (char *) message_date);
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, COLOR_DATE);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, BLACK);
		LB_Prev_Year(&today);
		LB_Date_to_Str(&today, (char *) message_date);
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, COLOR_DATE);
	}

	FSM_Event = event_none;
}

/**
  * @brief  sets a month
  * @param  None
  * @retval None
  */
void set_date_m(void)
{
	FSM_State = state_set_date_m;

	if (event_joystick_left == FSM_Event)
	{
		ssd1331_draw_line(Y_LINE_OFFSET_X, Y_LINE_OFFSET_Y, Y_LINE_OFFSET_X + LINE_LENGTH_YEAR_X, Y_LINE_OFFSET_Y, BLACK);
	}
	else if (event_joystick_right == FSM_Event)
	{
		 ssd1331_draw_line(D_LINE_OFFSET_X, D_LINE_OFFSET_Y, D_LINE_OFFSET_X + LINE_LENGTH_DAY_X, D_LINE_OFFSET_Y, BLACK);
	}

	ssd1331_draw_line(MONTH_LINE_OFFSET_X, MONTH_LINE_OFFSET_Y, MONTH_LINE_OFFSET_X + LINE_LENGTH_MONTH_X, MONTH_LINE_OFFSET_Y, COLOR_DATE);

	if (JOYSTICK_UP(Joystick))
	{
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, BLACK);
		LB_Next_Month(&today);
		LB_Date_to_Str(&today, (char *) message_date);
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, COLOR_DATE);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, BLACK);
		LB_Prev_Month(&today);
		LB_Date_to_Str(&today, (char *) message_date);
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, COLOR_DATE);
	}

	FSM_Event = event_none;
}

/**
  * @brief  sets a day
  * @param  None
  * @retval None
  */
void set_date_d(void)
{
	FSM_State = state_set_date_d;

	if (event_joystick_left == FSM_Event)
	{
		ssd1331_draw_line(MONTH_LINE_OFFSET_X, MONTH_LINE_OFFSET_Y, MONTH_LINE_OFFSET_X + LINE_LENGTH_MONTH_X, MONTH_LINE_OFFSET_Y, BLACK);
	}
	else if (event_joystick_right == FSM_Event)
	{
		ssd1331_draw_line(Y_LINE_OFFSET_X, Y_LINE_OFFSET_Y, Y_LINE_OFFSET_X + LINE_LENGTH_YEAR_X, Y_LINE_OFFSET_Y, BLACK);
	}

	ssd1331_draw_line(H_LINE_OFFSET_X, H_LINE_OFFSET_Y, END_OF_THE_LINE_TIME, H_LINE_OFFSET_Y, BLACK);
	ssd1331_draw_line(D_LINE_OFFSET_X, D_LINE_OFFSET_Y, D_LINE_OFFSET_X + LINE_LENGTH_DAY_X, D_LINE_OFFSET_Y, COLOR_DATE);

	if (JOYSTICK_UP(Joystick))
	{
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, BLACK);
		LB_Next_Day(&today);
		LB_Date_to_Str(&today, (char *) message_date);
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, COLOR_DATE);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, BLACK);
		LB_Prev_Day(&today);
		LB_Date_to_Str(&today, (char *) message_date);
		ssd1331_display_string(DATE_OFFSET_X, DATE_OFFSET_Y, message_date, FONT_1608, COLOR_DATE);
	}

	FSM_Event = event_none;
}

/**
  * @brief  sets a data logging period
  * @param  None
  * @retval None
  */
void set_data_logging_period(void)
{
	uint8_t message_logging_period[11];

	FSM_State = state_set_data_logging_period;
	ssd1331_draw_line(LINE_LOG_MINUTES_CENTER_X, LINE_LOG_MINUTES_CENTER_Y, EOL_LOG_MINUTES_LENGTH_X, LINE_LOG_MINUTES_CENTER_Y, DATA_LOGGING_COLOR_DIGITS);
	if (JOYSTICK_UP(Joystick))
	{
		sprintf( (char *) message_logging_period, "   %2u min", (logging_period[logging_index] / 60));
		ssd1331_display_string(LOG_MIN_CENTER_X, LOG_MIN_CENTER_Y, message_logging_period, FONT_1608, BLACK);
		if (++logging_index >= PERIOD_MAX)
		{
			logging_index = 0;
		}
		sprintf( (char *) message_logging_period, "   %2u min", (logging_period[logging_index] / 60));
		ssd1331_display_string(LOG_MIN_CENTER_X, LOG_MIN_CENTER_Y, message_logging_period, FONT_1608, DATA_LOGGING_COLOR_DIGITS);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		sprintf( (char *) message_logging_period, "   %2u min", (logging_period[logging_index] / 60));
		ssd1331_display_string(LOG_MIN_CENTER_X, LOG_MIN_CENTER_Y, message_logging_period, FONT_1608, BLACK);
		if (0 == logging_index--)
		{
			logging_index = PERIOD_MAX - 1;
		}
		sprintf( (char *) message_logging_period, "   %2u min", (logging_period[logging_index] / 60));
		ssd1331_display_string(LOG_MIN_CENTER_X, LOG_MIN_CENTER_Y, message_logging_period, FONT_1608, DATA_LOGGING_COLOR_DIGITS);
	}
	FSM_Event = event_none;
}

/** TODO: write the description
  * @brief  sets EVENT_e according to a joystick state
  * @param  None
  * @retval None
  */
void LB_DMA_Joystick_Event(const Joystick_t * p_Joystick, EVENT_e * p_event)
{
	if (!Joystick_Moved)
	{
		if (JOYSTICK_LEFT(*p_Joystick))
		{
			*p_event = event_joystick_left;
			Joystick_Moved = true;
		}
		else if (JOYSTICK_RIGHT(*p_Joystick))
		{
			*p_event = event_joystick_right;
			Joystick_Moved = true;
		}
	}
}
