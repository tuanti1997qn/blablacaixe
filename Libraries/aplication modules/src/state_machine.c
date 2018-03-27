#include "state_machine.h"

uint8_t ui8_State = 0;

void v_ImplementStateMachine( void )
{
    Kalman_Filter_Process();



}


uint8_t ui8_GetStateStatus ( void )
{
    return ui8_State;
}


void v_SetStateStatus ( uint8_t ui8_newstatus )
{
    ui8_State |= ui8_newstatus;
}

void v_ResetStateStatus ( uint8_t ui8_newstatus )
{
    ui8_State = ui8_newstatus;
}