#ifndef __TIMER_H
#define __TIMER_H

#include "main.h"

/* sample rate */
#define SAMPLE_RATE_MS 100.0
#define SAMPLE_RATE (84.0*SAMPLE_RATE_MS)
#define TIMER5_PRESCALER 999
#define TIMER5_PRESCALER_TO_CODE (TIMER5_PRESCALER-1)

/* config time_base use timer 5 */
void v_InitTimerBase ( void );
void v_TimerOverFlow ( void );
#endif