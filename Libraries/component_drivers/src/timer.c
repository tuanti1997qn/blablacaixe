#include "timer.h"

void v_TimerOverFlow( void );

static void v_configNVIC_timer5()
{
		NVIC_InitTypeDef stru_NVIC_Init;
    stru_NVIC_Init.NVIC_IRQChannel = TIM5_IRQn;
    stru_NVIC_Init.NVIC_IRQChannelCmd = ENABLE;
    /*stru_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 2;
    stru_NVIC_Init.NVIC_IRQChannelSubPriority = 2;*/
    NVIC_Init( &stru_NVIC_Init );//tai sao khi dung cai nay coa timer 4 thi lai gay bug. con nhung cai khac thi khong
}

void v_InitTimerBase ( void )
{
    RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM5 , ENABLE );

    TIM_TimeBaseInitTypeDef stru_InitTimeBase;
    stru_InitTimeBase.TIM_ClockDivision = TIM_CKD_DIV1; 
    stru_InitTimeBase.TIM_CounterMode = TIM_CounterMode_Up;
    stru_InitTimeBase.TIM_Period = SAMPLE_RATE;
    stru_InitTimeBase.TIM_Prescaler = TIMER5_PRESCALER;
    TIM_TimeBaseInit( TIM5 , &stru_InitTimeBase );

    TIM_SetCounter( TIM5 , 0 );
    TIM_ITConfig( TIM5 , TIM_IT_Update , ENABLE );

    /* config nvic */
    v_configNVIC_timer5();
	
    TIM_Cmd ( TIM5 , ENABLE );
}

void TIM5_IRQHandler( void )
{
    TIM_ClearITPendingBit( TIM5 , TIM_IT_Update );
    v_TimerOverFlow();
}