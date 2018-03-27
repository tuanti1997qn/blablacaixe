#ifndef __PWM_H
#define __PWM_H

#include "main.h"

#define TIM_PERIOD 840000/10
#define PRESCALER_VAL 99
#define DEFAULT_PULSE 8400

void v_PWM_Config(void);

#endif