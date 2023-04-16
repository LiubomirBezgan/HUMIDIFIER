/*
 * bme280_add.h
 *
 *  Created on: Mar 31, 2023
 *      Author: SKG.POTOP
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_BME280_ADD_H_
#define INC_BME280_ADD_H_

/* Includes ------------------------------------------------------------------*/
#include "bme280_defs.h"

/* General type definitions --------------------------------------------------*/
typedef enum {
	SI,
	Imperial
} UNITS_e;

// TODO: write descriptions
/* Function prototypes -------------------------------------------------------*/
/**
  * @brief  initializes a Time_t data type (time)
  * @param  ptime points to the Time_t data type (time)
  * @retval None
  */
int8_t BME280_init(struct bme280_dev *dev);

/**
  * @brief  initializes a Time_t data type (time)
  * @param  ptime points to the Time_t data type (time)
  * @retval None
  */
int8_t BME280_read_data (struct bme280_dev *dev, struct bme280_data * pcomp_data);

/**
  * @brief  initializes a Time_t data type (time)
  * @param  ptime points to the Time_t data type (time)
  * @retval None
  */
void delay_ms(uint32_t period, void *intf_ptr);

/**
  * @brief  initializes a Time_t data type (time)
  * @param  ptime points to the Time_t data type (time)
  * @retval None
  */
int8_t i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

/**
  * @brief  initializes a Time_t data type (time)
  * @param  ptime points to the Time_t data type (time)
  * @retval None
  */
int8_t i2c_write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

#endif /* INC_BME280_ADD_H_ */
