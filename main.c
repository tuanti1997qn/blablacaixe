#include "main.h"

volatile int16_t accel[3], gyro[3];
double data[2];
volatile uint16_t debug;
int main(void)
{
	v_Init_Mpu6050();
	GPIO_SetBits(GPIOD,GPIO_Pin_14);
	v_InitTimerBase();
	v_PWM_Config();
	v_InitEncoder();
	
	volatile int32_t count;
	uint8_t data[6] = {1,2,3,4,5,6};
	while(1)
	{
		/*if(i8_i2c_ReadMultiData(I2C1 , 0x68 , 0x75-5 , data , 6) == ERROR )
		{
			I2C_Cmd( I2C1 , DISABLE);
			count = 100;
			while(count--);
			I2C_Cmd(I2C1 , ENABLE);
		}*/
		//i8_i2c_ReadMultiData(I2C1 , 0x68 , 0x6B , data , 1);
		//i8_i2c_ReadMultiData(I2C1 , 0x68 , 0x19 , data , 6);
		//v_Mpu6050_get_Accel_Raw(accel);
		//v_Mpu6050_get_Gyro_Raw(gyro123);
		// count = 1000000;
		// while(count--);
		//i8_i2c_ReadMultiData(I2C1 , 0x68 , 0x6B , data , 1);
		//v_Mpu6050_get_Accel_Raw(accel);
	//	v_Mpu6050_get_Gyro_Raw(gyro);
		// MPU6050_Kalman_Angle(data);

		/*----------------------------------------------------*/
		debug = TIM_GetCounter( TIM2 );
		/*----------------------------------------------------*/
		if( ui8_GetStateStatus() )
		{
			v_ImplementStateMachine();
			v_ResetStateStatus( STATUS_CLEAR );
		}
		MPU6050_Kalman_Angle(data);
	}
	
}







