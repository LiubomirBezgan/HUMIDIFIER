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
	sprintf( (char *) message_logging_period, "   LOGGING");
	ssd1331_display_string(DATA_LOGGING_CENTER_X, DATA_LOGGING_CENTER_Y, message_logging_period, FONT_1608, GOLDEN);
	sprintf( (char *) message_logging_period, "   PERIOD:");
	ssd1331_display_string(PERIOD_CENTER_X, PERIOD_CENTER_Y, message_logging_period, FONT_1608, GOLDEN);
	sprintf( (char *) message_logging_period, "    %2d min", (logging_period[logging_index] / 60));
	ssd1331_display_string(MIN_CENTER_X, MIN_CENTER_Y, message_logging_period, FONT_1608, GOLDEN);

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
	ssd1331_draw_line(33, 63, 48, 63, GOLDEN); // TODO: define line constants
	if (JOYSTICK_UP(Joystick))
	{
		sprintf( (char *) message_logging_period, "    %2d min", (logging_period[logging_index] / 60));
		ssd1331_display_string(MIN_CENTER_X, MIN_CENTER_Y, message_logging_period, FONT_1608, BLACK);
		if (++logging_index >= PERIOD_MAX)
		{
			logging_index = 0;
		}
		sprintf( (char *) message_logging_period, "    %2d min", (logging_period[logging_index] / 60));
		ssd1331_display_string(MIN_CENTER_X, MIN_CENTER_Y, message_logging_period, FONT_1608, GOLDEN);
	}
	else if (JOYSTICK_DOWN(Joystick))
	{
		sprintf( (char *) message_logging_period, "    %2d min", (logging_period[logging_index] / 60));
		ssd1331_display_string(MIN_CENTER_X, MIN_CENTER_Y, message_logging_period, FONT_1608, BLACK);
		if (0 == logging_index--)
		{
			logging_index = PERIOD_MAX - 1;
		}
		sprintf( (char *) message_logging_period, "    %2d min", (logging_period[logging_index] / 60));
		ssd1331_display_string(MIN_CENTER_X, MIN_CENTER_Y, message_logging_period, FONT_1608, GOLDEN);
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
