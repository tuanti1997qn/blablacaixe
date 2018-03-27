#ifndef __MPU6050_H
#define __MPU6050_H

#include "main.h"
#include "i2c.h"

#define MPU6050_SMPLRT_DIV                  0x19
#define MPU6050_INT_PIN_CFG                 0x37
#define MPU6050_ACCEL_XOUT_H                0x3B
#define MPU6050_GYRO_XOUT_H                 0x43
#define MPU6050_PWR_MGMT_1                  0x6B
#define MPU6050_WHO_AM_I                    0x75

#define MPU6050_ADDRESS                     0x68
#define MPU6050_WHO_AM_I_ID                 0x68

// Scale factor for +-2000deg/s and +-8g - see datasheet:
#define MPU6050_GYRO_SCALE_FACTOR_2000      16.4f
#define MPU6050_ACC_SCALE_FACTOR_8          4096.0f

#define RAD_TO_DEG                          (180/3.14159)

/* 	in this case:
		- measure maximum 2000 deg/s, 8g 
		- use I2C1 PB6 = SCL; PB7 = SDA 
		- sample rate : 1kHz
		- 41 Hz filt accel & gyro 	*/
void v_Init_Mpu6050();
void v_Mpu6050_get_Accel_Raw(int16_t* data);
void v_Mpu6050_get_Gyro_Raw(int16_t* data);

#endif /* __MPU6050_H */