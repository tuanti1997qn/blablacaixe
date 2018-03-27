#ifndef __I2C_H
#define __I2C_H

#include "main.h"

#define time_error 1000


int8_t i8_i2c1_Init( uint32_t i2c_ClockSpeed , uint16_t i2c_dutycycles );
int8_t i8_i2c_GenStart(I2C_TypeDef* I2Cx, uint8_t ui8_address, uint8_t direction);
int8_t i8_i2c_GenStop( I2C_TypeDef* I2Cx);
int8_t i8_i2c_ReadMultiData(I2C_TypeDef* I2Cx, uint8_t slave_address,uint8_t reg_address , uint8_t *data, uint8_t data_length);
int8_t i8_i2c_SendMultiData(I2C_TypeDef* I2Cx, uint8_t slave_address, uint8_t reg_address , uint8_t *data, uint8_t data_length);
void v_i2c_reset(I2C_TypeDef* I2Cx);


#endif