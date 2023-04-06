/*
 * LB_date&time_add.h
 *
 *  Created on: 6 apr. 2023
 *      Author: SKG.POTOP
 */

#ifndef INC_LB_DATE_TIME_ADD_H_
#define INC_LB_DATE_TIME_ADD_H_

#include "SSD1331.h"

#define COLOR_TIME GREEN
#define COLOR_DATE PURPLE
#define CENTER_X 17
#define CENTER_Y 35
#define LINE_OFFSET_X 0
#define LINE_OFFSET_Y 17
#define LINE_PITCH_X 24
#define LINE_LENGTH_X 15
#define LINE_LENGTH_DAY_X 15
#define LINE_LENGTH_MONTH_X 24
#define LINE_LENGTH_YEAR_X 31
#define END_OF_THE_LINE_TIME ( (CENTER_X) + ( (LINE_PITCH_X) * 2 ) + LINE_LENGTH_X )
#define END_OF_THE_LINE_DATE (DATE_OFFSET_X + LINE_OFFSET_X + YEAR_OFFSET + LINE_LENGTH_YEAR_X)

#define H_LINE_OFFSET_X (CENTER_X + LINE_OFFSET_X)
#define H_LINE_OFFSET_Y (CENTER_Y + LINE_OFFSET_Y)
#define M_LINE_OFFSET_X (CENTER_X + LINE_OFFSET_X + LINE_PITCH_X)
#define M_LINE_OFFSET_Y (CENTER_Y + LINE_OFFSET_Y)
#define S_LINE_OFFSET_X (CENTER_X + LINE_OFFSET_X + (LINE_PITCH_X * 2))
#define S_LINE_OFFSET_Y (CENTER_Y + LINE_OFFSET_Y)

#define D_LINE_OFFSET_X (DATE_OFFSET_X + LINE_OFFSET_X)
#define D_LINE_OFFSET_Y (DATE_OFFSET_Y + LINE_OFFSET_Y)
#define MONTH_LINE_OFFSET_X (DATE_OFFSET_X + LINE_OFFSET_X + LINE_PITCH_X)
#define MONTH_LINE_OFFSET_Y (DATE_OFFSET_Y + LINE_OFFSET_Y)
#define YEAR_OFFSET (LINE_LENGTH_DAY_X + LINE_LENGTH_MONTH_X + LINE_LENGTH_X + 2)
#define Y_LINE_OFFSET_X (DATE_OFFSET_X + LINE_OFFSET_X + YEAR_OFFSET)
#define Y_LINE_OFFSET_Y (DATE_OFFSET_Y + LINE_OFFSET_Y)

#define DATE_OFFSET_X (CENTER_X - 13)
#define DATE_OFFSET_Y (CENTER_Y - 20)


#define LOGGING_CENTER_X 17
#define LOGGING_CENTER_Y 35
#define LOGGING_LINE_OFFSET_X (LOGGING_CENTER_X + LINE_OFFSET_X)
#define LOGGING_LINE_OFFSET_Y (LOGGING_CENTER_Y + LINE_OFFSET_Y)

#endif /* INC_LB_DATE_TIME_ADD_H_ */