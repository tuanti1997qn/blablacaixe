#ifndef __KALMAN_FILTER_H
#define __KALMAN_FILTER_H

#include "main.h"

#define MPU6050_GYRO_SCALE_FACTOR_2000      16.4f
#define MPU6050_ACC_SCALE_FACTOR_8          4096.0f

#define RAD_TO_DEG                          (180/3.14159)
#define dt_update                           (SAMPLE_RATE_MS/1000.0)

/* funciton decription */
void Kalman_Filter_Process(void);
void MPU6050_Kalman_Angle(double * d_angle);
void MPU6050_Complimentary_Angle(double * d_angle);
void Kalman_Set_Angle(float angle);
void Kalman_Set_Qangle(float Q_angle);
void Kalman_Set_Qbias(float Q_bias);
void Kalman_Set_Rmeasure(float R_measure);
float Kalman_Get_Qangle(void);
float Kalman_Get_Qbias(void);
float Kalman_Get_Rmeasure(void);

#endif