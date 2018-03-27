#include "mpu6050.h"

/* define function */
static void v_ConfigLed( void );

/* function */
void v_Init_Mpu6050()
{
    v_ConfigLed();
    int8_t check_error = ERROR;
		i8_i2c1_Init( 200000 , I2C_DutyCycle_16_9 );
    /* init i2c to read imu */
		while(check_error == ERROR)
    /* loop to success*/
    {
			ReInit:
        v_i2c_reset( I2C1 );
        //check_error = i8_i2c1_Init(400000 , I2C_DutyCycle_16_9);

        uint8_t au8_i2cBuffer[5];
        /* reset mpu6050 to default value */
        au8_i2cBuffer[0] = 1<<7;
        check_error = i8_i2c_SendMultiData(I2C1 , MPU6050_ADDRESS , MPU6050_PWR_MGMT_1 , au8_i2cBuffer , 1 );
			
        /* while to complete */
        au8_i2cBuffer[0] = 1<<7;
				do
        {
            check_error = i8_i2c_ReadMultiData(I2C1 , MPU6050_ADDRESS , MPU6050_PWR_MGMT_1 , au8_i2cBuffer , 1);
        }
				while( au8_i2cBuffer[0] & 1<<7 );
				/* bug */
				if(au8_i2cBuffer[0] == 0x00)
				{
					goto ReInit;
				}
				
        /* Disable sleep mode, disable temperature sensor and use PLL as clock reference */
        au8_i2cBuffer[0] = (1<<3)|(1<<0);
        check_error = i8_i2c_SendMultiData(I2C1 , MPU6050_ADDRESS , MPU6050_PWR_MGMT_1 , au8_i2cBuffer , 1);
        au8_i2cBuffer[0] = 0; // Set the sample rate to 1kHz - 1kHz/(1+0) = 1kHz
        au8_i2cBuffer[1] = 0x03; // Disable FSYNC and set 41 Hz Gyro filtering, 1 KHz sampling
        au8_i2cBuffer[2] = 3 << 3; // Set Gyro Full Scale Range to +-2000deg/s
        au8_i2cBuffer[3] = 2 << 3; // Set Accelerometer Full Scale Range to +-8g
        au8_i2cBuffer[4] = 0x03; // 41 Hz Acc filtering
        check_error = i8_i2c_SendMultiData(I2C1 , MPU6050_ADDRESS , MPU6050_SMPLRT_DIV , au8_i2cBuffer , 5 );
								
        /* Enable Raw Data Ready Interrupt on INT pin */
        au8_i2cBuffer[0] = (1 << 5) | (1 << 4); // Enable LATCH_INT_EN and INT_ANYRD_2CLEAR
        // When this bit is equal to 1, the INT pin is held high until the interrupt is cleared
        // When this bit is equal to 1, interrupt status is cleared if any read operation is performed
        au8_i2cBuffer[1] = (1 << 0);            // Enable RAW_RDY_EN - When set to 1, Enable Raw Sensor Data Ready interrupt to propagate to interrupt pin
        check_error = i8_i2c_SendMultiData(I2C1 , MPU6050_ADDRESS , MPU6050_INT_PIN_CFG , au8_i2cBuffer , 2 ); // Write to both registers at once
    }
}

void v_Mpu6050_get_Accel_Raw(int16_t* data)
{
    uint8_t au8_buff[6];
    while(i8_i2c_ReadMultiData(I2C1 , MPU6050_ADDRESS , MPU6050_ACCEL_XOUT_H , au8_buff , 6) == ERROR )
		{
			v_i2c_reset(I2C1);
		}
    data[0] = (au8_buff[0] << 8) | au8_buff[1];    //Accel X
    data[1] = (au8_buff[2] << 8) | au8_buff[3];    //Accel Y
    data[2] = (au8_buff[4] << 8) | au8_buff[5];    //Accel Z
}

void v_Mpu6050_get_Gyro_Raw(int16_t* data)
{
    uint8_t au8_buff[6];
    while(i8_i2c_ReadMultiData(I2C1 , MPU6050_ADDRESS , MPU6050_GYRO_XOUT_H , au8_buff , 6) == ERROR )
		{
			v_i2c_reset(I2C1);
		}
    data[0] = (au8_buff[0] << 8) | au8_buff[1];    //Accel X
    data[1] = (au8_buff[2] << 8) | au8_buff[3];    //Accel Y
    data[2] = (au8_buff[4] << 8) | au8_buff[5];    //Accel Z 
}


void v_TimerOverFlow( void )
{
    static int8_t i8_toggle_led;
    if(i8_toggle_led == 0){
        i8_toggle_led = 1;
        GPIO_SetBits( GPIOD , GPIO_Pin_14 );
    }
    else{
        i8_toggle_led = 0;
        GPIO_ResetBits( GPIOD , GPIO_Pin_14 );
    }
    v_SetStateStatus( 1 );
}

/* static function ----------------------------------------------------------------------- */

/* use led PD14 */
static void v_ConfigLed( void )
{
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE);
    
    GPIO_InitTypeDef stru_InitGpio;
    stru_InitGpio.GPIO_Mode = GPIO_Mode_OUT;
    stru_InitGpio.GPIO_OType = GPIO_OType_PP;
    stru_InitGpio.GPIO_Pin = GPIO_Pin_14;
    stru_InitGpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOD , &stru_InitGpio );
}



