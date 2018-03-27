#include "PWM.h"

void v_PWM_Config(void)
{
    /*config GPIO pin*/
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE );
    GPIO_InitTypeDef stru_GpioInit;
    stru_GpioInit.GPIO_Mode  = GPIO_Mode_AF;
    stru_GpioInit.GPIO_OType = GPIO_OType_PP;
    stru_GpioInit.GPIO_Pin   = GPIO_Pin_1;
    stru_GpioInit.GPIO_PuPd  = GPIO_PuPd_UP;
    stru_GpioInit.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init( GPIOA , &stru_GpioInit);
    GPIO_PinAFConfig( GPIOA , GPIO_PinSource1 , GPIO_AF_TIM2);

    /* config timer, PWM*/
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2 , ENABLE );
    TIM_TimeBaseInitTypeDef stru_TimeBaseInit;
    stru_TimeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
    stru_TimeBaseInit.TIM_CounterMode   = TIM_CounterMode_Up;
    stru_TimeBaseInit.TIM_Period        = TIM_PERIOD;
    stru_TimeBaseInit.TIM_Prescaler     = PRESCALER_VAL;
    TIM_TimeBaseInit( TIM2 , &stru_TimeBaseInit);

    /*config PWM mode(OC)*/
    TIM_OCInitTypeDef stru_OcInit;
    stru_OcInit.TIM_OCIdleState = TIM_OCIdleState_Reset;
    stru_OcInit.TIM_OCMode      = TIM_OCMode_PWM1;
    stru_OcInit.TIM_OCIdleState = TIM_OCIdleState_Set;
    stru_OcInit.TIM_OCPolarity  = TIM_OCPolarity_High; /* chua hieu lam */
   // stru_OcInit.TIM_OCPolarity  = TIM_OCPolarity_High; /* cung không hieu */
    stru_OcInit.TIM_OutputState = TIM_OutputState_Enable;
	stru_OcInit.TIM_Pulse   		= DEFAULT_PULSE;
    TIM_OC2Init( TIM2 , &stru_OcInit );
    //TIM_OC2PreloadConfig( TIM2 , TIM_OCPreload_Enable );

    TIM_Cmd( TIM2 , ENABLE );
} 

void v_SetPulse( uint16_t ui16_Pulse )
{
    TIM2->CCR2 = ui16_Pulse;
}
