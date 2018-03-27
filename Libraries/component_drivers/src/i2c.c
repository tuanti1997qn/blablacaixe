#include "i2c.h"


int8_t i8_i2c1_Init( uint32_t i2c_ClockSpeed , uint16_t i2c_dutycycles )
{
    // config pin
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    GPIO_InitTypeDef stru_init_gpio;
    stru_init_gpio.GPIO_Mode = GPIO_Mode_AF;
    stru_init_gpio.GPIO_OType = GPIO_OType_OD;
    stru_init_gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
    stru_init_gpio.GPIO_Speed = GPIO_Speed_50MHz;
    stru_init_gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&stru_init_gpio);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

    I2C_InitTypeDef stru_init_i2c;
    stru_init_i2c.I2C_Ack = I2C_Ack_Disable;
    stru_init_i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    stru_init_i2c.I2C_ClockSpeed = i2c_ClockSpeed; 
    stru_init_i2c.I2C_DutyCycle = i2c_dutycycles;
    stru_init_i2c.I2C_Mode = I2C_Mode_I2C;
    I2C_Init(I2C1,&stru_init_i2c);

    I2C_Cmd(I2C1,ENABLE);
	
		return SUCCESS;
}

int8_t i8_i2c_GenStart(I2C_TypeDef* I2Cx, uint8_t ui8_address, uint8_t direction)
{
    int16_t i16_count_error;
	// wait until I2C1 is not busy anymore
    /*i16_count_error = time_error;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    {
        if((i16_count_error--) == 0 )
        {
            return ERROR;
        }
    }*/
  
	// Send I2C1 START condition 
	I2C_GenerateSTART(I2Cx, ENABLE);
	  
	// wait for I2C1 EV5 --> Slave has acknowledged start condition
    i16_count_error = time_error;
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((i16_count_error--) == 0 )
        {
            return ERROR;
        }
    }
		
	// Send slave Address for write 
	I2C_Send7bitAddress(I2Cx, ui8_address<<1 , direction);
	  
	/* wait for I2C1 EV6, check if 
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */ 
	if(direction == I2C_Direction_Transmitter)
    {
        i16_count_error = time_error;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        {
            if((i16_count_error--) == 0 )
            {
                return ERROR;
            }
        }
	}
	else if(direction == I2C_Direction_Receiver)
    {
        i16_count_error = time_error;
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        {
            if((i16_count_error--) == 0 )
            {
                return ERROR;
            }
        }
	}
    return SUCCESS;
}

// generate stop condition
int8_t i8_i2c_GenStop( I2C_TypeDef* I2Cx)
{
    /*int16_t i16_count_error = time_error;
    // while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF))))
    while ( I2C_CheckEvent( I2Cx , I2C_EVENT_MASTER_BYTE_TRANSMITTED ) == ERROR )
    {
        if (--i16_count_error == 0x00)
        {
            return ERROR;
        }
	}*/

    I2C_GenerateSTOP( I2Cx, ENABLE );
    return SUCCESS;
}

int8_t i8_i2c_SendData ( I2C_TypeDef* I2Cx , uint8_t* ui8_datasend )
{
    I2C_SendData(I2Cx , *ui8_datasend );
    int16_t i16_count_error = time_error;
    while( I2C_CheckEvent( I2Cx , I2C_EVENT_MASTER_BYTE_TRANSMITTED ) == ERROR )
    {
        if(--i16_count_error == 0x00)
        {
            return ERROR;
        }
    }
    return SUCCESS;
}

int8_t i8_i2c_GetData_ACK(I2C_TypeDef* I2Cx, uint8_t* ui8_dataget)
{
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    int16_t i16_count_error = time_error;
    // while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF))))
    while ( I2C_CheckEvent( I2Cx , I2C_EVENT_MASTER_BYTE_RECEIVED ) == ERROR )
    {
        if (--i16_count_error == 0x00)
        {
            return ERROR;
        }
	}
    *ui8_dataget = I2C_ReceiveData(I2Cx);
    return SUCCESS;
}

int8_t i8_i2c_GetData_NACK(I2C_TypeDef* I2Cx, uint8_t* ui8_dataget)
{
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    int16_t i16_count_error = time_error;
    // while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF))))
    while ( I2C_CheckEvent( I2Cx , I2C_EVENT_MASTER_BYTE_RECEIVED ) == ERROR )
    {
        if (--i16_count_error == 0x00)
        {
            return ERROR;
        }
	}
    *ui8_dataget = I2C_ReceiveData(I2Cx);
    return SUCCESS;
}

int8_t i8_i2c_SendMultiData(I2C_TypeDef* I2Cx, uint8_t slave_address, uint8_t reg_address , uint8_t *data, uint8_t data_length)
{
    int16_t i16_count_error;
    i16_count_error = time_error;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    {
        if((i16_count_error--) == 0 )
        {
            return ERROR;
        }
    }
    i8_i2c_GenStart(I2Cx , slave_address , I2C_Direction_Transmitter );
    i8_i2c_SendData(I2Cx , &reg_address);
    int8_t count_data;
    for(count_data = 0 ; count_data < data_length ; count_data++ )
    {
        i8_i2c_SendData(I2Cx , data + count_data);
    }
    i8_i2c_GenStop(I2Cx);
		return SUCCESS;
}

int8_t i8_i2c_ReadMultiData(I2C_TypeDef* I2Cx, uint8_t slave_address,uint8_t reg_address , uint8_t *data, uint8_t data_length)
{
    int16_t i16_count_error;
    i16_count_error = time_error;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    {
        if((i16_count_error--) == 0 )
        {
            return ERROR;
        }
    }
    i8_i2c_GenStart(I2Cx , slave_address , I2C_Direction_Transmitter );
    i8_i2c_SendData(I2Cx , &reg_address);
    int16_t count_err =100;
    while(count_err-- );
		int8_t count_data;
    i8_i2c_GenStart(I2Cx , slave_address , I2C_Direction_Receiver ); 	
    for(count_data = 0 ; count_data < data_length-1 ; count_data++ )
    {
        i8_i2c_GetData_ACK(I2Cx , data + count_data);
    }
    i8_i2c_GetData_NACK(I2Cx , data+data_length-1);
    i8_i2c_GenStop(I2Cx);
    return SUCCESS;
}

void v_i2c_reset(I2C_TypeDef* I2Cx)
{
    I2C_Cmd(I2Cx , DISABLE);
    I2C_Cmd(I2Cx , ENABLE);
}