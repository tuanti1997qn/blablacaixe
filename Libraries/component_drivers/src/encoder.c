#include "encoder.h"

/* global variable */
int32_t cycles;//dat cai gi cho ngau nhe


/* external function */

void v_InitEncoder (void)
{
    /* config pin*/
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE );
    
    GPIO_InitTypeDef stru_InitGpio;
    stru_InitGpio.GPIO_Mode = GPIO_Mode_AF;
    stru_InitGpio.GPIO_Pin = ENCODER_PIN_A | ENCODER_PIN_B;
    stru_InitGpio.GPIO_PuPd = GPIO_PuPd_UP;
    stru_InitGpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init ( GPIOA , &stru_InitGpio );
    
    GPIO_PinAFConfig( GPIOA , PIN_SOURCE_ENC_A , GPIO_AF_TIM2 );
    GPIO_PinAFConfig( GPIOA , PIN_SOURCE_ENC_B , GPIO_AF_TIM2 );

    /*con fig timer base */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2 , ENABLE );

    TIM_TimeBaseInitTypeDef stru_InitTimeBase;
    stru_InitTimeBase.TIM_ClockDivision = TIM_CKD_DIV1;
    stru_InitTimeBase.TIM_CounterMode = TIM_CounterMode_Up;
    stru_InitTimeBase.TIM_Period = ENCODER_PERIOD;
    stru_InitTimeBase.TIM_Prescaler = 0;
    TIM_TimeBaseInit( TIM2 , &stru_InitTimeBase );

    /* config interrupt */
    TIM_SetCounter( TIM2 , DEFAULT_VALUE_COUNTER );
    TIM_ITConfig( TIM2 , TIM_IT_Update , ENABLE );

    TIM_EncoderInterfaceConfig( TIM2 , TIM_EncoderMode_TI1 , TIM_ICPolarity_Rising , TIM_ICPolarity_Rising );


    /* config NVIC to reset whenever timer count overflow */
    NVIC_InitTypeDef stru_InitNVIC;
    stru_InitNVIC.NVIC_IRQChannel = TIM2_IRQn;
    stru_InitNVIC.NVIC_IRQChannelPreemptionPriority = 1;
    stru_InitNVIC.NVIC_IRQChannelSubPriority = 1;
    stru_InitNVIC.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &stru_InitNVIC );

    TIM_Cmd( TIM2 , ENABLE);
}
 

int32_t i32_GetCycles( void )
{
    return cycles;
}

void v_SetCycles( int32_t i32_value )
{
    cycles = i32_value;
}

int32_t i32_GetEncoderValue( void )
{
    return TIM_GetCounter( TIM2 );
}

void v_SetEncoderValue( uint32_t ui32_SetValue )
{
    TIM_SetCounter( TIM2, ui32_SetValue );
}

/* static function*/
static void TIM2_IRQHandler( void )
{
    /* sau nay se sua lai nhu cai kia la ham nay goi mot ham khac. dua cai ham kia ra de handle*/
    TIM_ClearITPendingBit( TIM2 , TIM_IT_Update );
    v_EncoderHandle();
}

#ifndef "USE_EXTERNAL_HANDLE"
    void v_EncoderHandle( void )
    {
        if( TIM_GetCounter( TIM2 ) <= 20 )
        {
            cycles++; // neu tran ve 0 thi ra the
        }
        else
        {
            cycles--; // neu trantu 0 len FFFFFFFF thì tru
        }
    }
#endif