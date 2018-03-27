#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"


//#define USE_EXTERNAL_HANDLE  /* comment this line to use v_EncoderHandle */

/* macro(?) */
#define ENCODER_PERIOD 0xFFFF
#define ENCODER_PIN_A GPIO_Pin_0
#define ENCODER_PIN_B GPIO_Pin_1
#define PIN_SOURCE_ENC_A GPIO_PinSource0
#define PIN_SOURCE_ENC_B GPIO_PinSource1
#define DEFAULT_VALUE_COUNTER 0


/* funtion */
void v_InitEncoder (void);
int32_t i32_GetCycles( void );
void v_SetCycles( int32_t i32_value );
int32_t i32_GetEncoderValue( void );
void v_SetEncoderValue( uint32_t ui32_SetValue;
void v_EncoderHandle( void );

#endif