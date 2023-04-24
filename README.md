# HUMIDITY CONTROL INDOOR STATION

STM32F411RE \ Bare metal \ Finite-state machine \ STM32CubeIDE

![Figure 1](https://github.com/LiubomirBezgan/HUMIDIFIER/blob/main/listed_devices.png, "Humidity control indoor station mounted on two bread boards and supplied by a mini-USB cable.")
Figure 1. Humidity control indoor station mounted on two bread boards and supplied by a mini-USB cable.

The device presented in Figure 1. is used to control and register indoor humidity level. An air humidification process is executed by ultrasonic membrane humidifier. User can adjust target humidity level, the duration of humidification and the delay between two consequent humidification processes. BME280 temperature, humidity and pressure sensor is used for data acquisition which allows user to check current indoor conditions printed on the OLED display. User can log the data to SD card and the log period is adjustable as well. Log data consists of date, time, temperature in °C, humidity in % and atmospheric pressure in mmHg. The device requires 5V DC power supply.

+ MCU Architecture: **Arm** 32-bit **Cortex-M4** CPU with FPU (STM32F411RE)
+ The type of programming: **bare-metal**
+ Main algorithm: **finite-state machine** (FSM)
+ Peripheries:
  + BME280 temperature, humidity and pressure sensor (**I2C**),
  + Ultrasonic membrane humidifier (**GPIO**),
  + SSD1331 OLED display (**SPI**),
  + Joystick (**ADC**, **DMA**),
  + SD Card (**FatFS**, **SPI**),
  + Clock and calendar (**timer**, **library development**).
  
_____
### FINITE-STATE MACHINE
The main behavior of the device is determined by state transition table presented in Listing 1. The program switches states in the super loop according to joystick pushes and movements (see more details in [LB_FSM_Humidifier.h](https://github.com/LiubomirBezgan/HUMIDIFIER/blob/main/STM32_HUMIDIFIER/Core/Inc/LB_FSM_Humidifier.h) and [LB_FSM_Humidifier.c](https://github.com/LiubomirBezgan/HUMIDIFIER/blob/main/STM32_HUMIDIFIER/Core/Src/LB_FSM_Humidifier.c)).
 ```c
TRANSITION_FUNC_PTR_t LB_Transition_Table[STATE_MAX][EVENT_MAX] = {
		[state_thp_screen]			[event_none]		=thp_screen,
		[state_thp_screen]			[event_button_pressed]	=thp_screen,
		[state_thp_screen]			[event_joystick_right]	=humidifier_screen,
		[state_thp_screen]			[event_joystick_left]	=data_logging_period_screen,

		[state_humidifier_screen]		[event_none]		=humidifier_screen,
		[state_humidifier_screen]		[event_button_pressed]	=set_humidity,
		[state_humidifier_screen]		[event_joystick_right]	=clock_screen,
		[state_humidifier_screen]		[event_joystick_left]	=thp_screen,

		[state_clock_screen]			[event_none]		=clock_screen,
		[state_clock_screen]			[event_button_pressed]	=set_time_h,
		[state_clock_screen]			[event_joystick_right]	=data_logging_period_screen,
		[state_clock_screen]			[event_joystick_left]	=humidifier_screen,

		[state_data_logging_period_screen]	[event_none]		=data_logging_period_screen,
		[state_data_logging_period_screen]	[event_button_pressed]	=set_data_logging_period,
		[state_data_logging_period_screen]	[event_joystick_right]	=thp_screen,
		[state_data_logging_period_screen]	[event_joystick_left]	=clock_screen,

		[state_set_humidity]			[event_none]		=set_humidity,
		[state_set_humidity]			[event_button_pressed]	=set_duration,
		[state_set_humidity]			[event_joystick_right]	=set_humidity,
		[state_set_humidity]			[event_joystick_left]	=set_humidity,

		[state_set_duration]			[event_none]		=set_duration,
		[state_set_duration]			[event_button_pressed]	=set_delay,
		[state_set_duration]			[event_joystick_right]	=set_duration,
		[state_set_duration]			[event_joystick_left]	=set_duration,

		[state_set_delay]			[event_none]		=set_delay,
		[state_set_delay]			[event_button_pressed]	=humidifier_screen,
		[state_set_delay]			[event_joystick_right]	=set_delay,
		[state_set_delay]			[event_joystick_left]	=set_delay,

		[state_set_time_h]			[event_none]		=set_time_h,
		[state_set_time_h]			[event_button_pressed]	=set_date_d,
		[state_set_time_h]			[event_joystick_right]	=set_time_m,
		[state_set_time_h]			[event_joystick_left]	=set_time_s,

		[state_set_time_m]			[event_none]		=set_time_m,
		[state_set_time_m]			[event_button_pressed]	=set_date_d,
		[state_set_time_m]			[event_joystick_right]	=set_time_s,
		[state_set_time_m]			[event_joystick_left]	=set_time_h,

		[state_set_time_s]			[event_none]		=set_time_s,
		[state_set_time_s]			[event_button_pressed]	=set_date_d,
		[state_set_time_s]			[event_joystick_right]	=set_time_h,
		[state_set_time_s]			[event_joystick_left]	=set_time_m,

		[state_set_date_y]			[event_none]		=set_date_y,
		[state_set_date_y]			[event_button_pressed]	=clock_screen,
		[state_set_date_y]			[event_joystick_right]	=set_date_d,
		[state_set_date_y]			[event_joystick_left]	=set_date_m,

		[state_set_date_m]			[event_none]		=set_date_m,
		[state_set_date_m]			[event_button_pressed]	=clock_screen,
		[state_set_date_m]			[event_joystick_right]	=set_date_y,
		[state_set_date_m]			[event_joystick_left]	=set_date_d,

		[state_set_date_d]			[event_none]		=set_date_d,
		[state_set_date_d]			[event_button_pressed]	=clock_screen,
		[state_set_date_d]			[event_joystick_right]	=set_date_m,
		[state_set_date_d]			[event_joystick_left]	=set_date_y,

		[state_set_data_logging_period]		[event_none]		=set_data_logging_period,
		[state_set_data_logging_period]		[event_button_pressed]	=data_logging_period_screen,
		[state_set_data_logging_period]		[event_joystick_right]	=set_data_logging_period,
		[state_set_data_logging_period]		[event_joystick_left]	=set_data_logging_period

};
```
Listing 1. The implementation of state transition table.

There are three **interrupt** handlers in the program as well. TIM10 handler ensures all time operations (clock, calendar, active state duration, logging period etc.). EXIT4 handler is responsible for the joystick button state control. DMA2 handler reacts to joystick’s left-right movements. You can see more details in [stm32f4xx_it.c](https://github.com/LiubomirBezgan/HUMIDIFIER/blob/main/STM32_HUMIDIFIER/Core/Src/stm32f4xx_it.c).

### USER INTERFACE
The user interface consists of the 96x64 pixels SSD1331 **OLED display** and the **joystick** which has a button activated by pressing the handle.
There are **4 menus** printed on OLED display: data screen, humidifier parameters screen, date and time screen, logging parameter screen (Figure 2.). User can swipe menus by moving joystick left or right.

![Figure 2.](https://github.com/LiubomirBezgan/HUMIDIFIER/blob/main/displays_OLED__edited.jpg, "Figure 2. Four user interface menus: data menu presents temperature (1.1. and 1.2), humidifier parameters menu (2.1. and 2.2), date and time menu (3.1. and 3.2), logging parameter menu (4.1. and 4.2).")
Figure 2. Four user interface menus: data menu presents temperature (1.1. and 1.2), humidifier parameters menu (2.1. and 2.2), date and time menu (3.1. and 3.2), logging parameter menu (4.1. and 4.2).

The **data menu** presents temperature, humidity and atmospheric pressure data and allows user to change units by pushing the joystick button. There are two unit sets. The first set consists of degree Celsius, relative humidity (%) and millimeter of mercury (Figure 2.1.1.). The units of the second set are degree Fahrenheit, relative humidity (%) and pascal (Figure 2.1.2.).

The **humidifier parameters menu** shows current target humidity level, the duration of humidification and the delay between two consequent humidification processes (Figure 2.2.1.). Pushing the joystick button leads to state when user can adjust target humidity level by moving joystick up-down (Figure 2.2.2.). If user wants to set the duration of humidification, he has to push the joystick button one more time and commits the same actions. Next joystick button pushing leads to the delay state set. Forth push allows user to come back to the humidifier parameters menu and to change the menu.

The **date and time menu** prints current date and time (Figure 2.3.1.). Pushing the joystick button leads to time adjustment state (Figure 2.3.2.). In the state user can set current time by joystick movement (left-right to change time parameter (hours, minutes or seconds); up-down to increment/decrement specified parameter). Next joystick button pushing submits time and bring device to date adjustment state with the same rules. Third push allows user to come back to the date and time menu.

The **logging parameter menu** is used for check current logging period (Figure 2.4.1.). It can be changed by pushing the joystick button and up-down joystick movement. The follow logging periods are available: 1, 5, 10, 15, 20 and 30 minutes (Figure 2.4.2.).


### LOG SYSTEM
The log system is based on **FatFS** and implemented by writing date, time, temperature in °C, humidity in % and atmospheric pressure in mmHg data to the [logs.csv](https://github.com/LiubomirBezgan/HUMIDIFIER/blob/main/logs.csv) file stored on a **SD card**. Device continues to log data after SD card re-insertion or creates new logs.csv file in the file absence case. The .csv format allows for the fast preparation of logged data for further analyzes (Figure 3.).

![Figure 3.](https://github.com/LiubomirBezgan/HUMIDIFIER/blob/main/log%20file.png, "Figure 3. The readability improving preparation of logs.csv in Excel")
Figure 3. The readability improving preparation of logs.csv in Excel

### FUTURE PERSPECTIVES
+ New features adding:
  + Logging the average values of sensor data,
  + The lowest and the highest values recording,
  + The adjustable format of log data.
+ Optimazing:
  + using integer sensor data values instead of float values (the integer version of BME280 sensor API)
  + 
