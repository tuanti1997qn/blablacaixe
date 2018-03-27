#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include "main.h"

#define STATUS_CLEAR 0


/* function description */
void v_ImplementStateMachine( void );
uint8_t ui8_GetStateStatus ( void );
void v_SetStateStatus ( uint8_t ui8_newstatus );
void v_ResetStateStatus ( uint8_t ui8_newstatus );

#endif