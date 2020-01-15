/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../elevator/elevator.h"
#include "../button_EXINT/button.h"
#include "../const.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../adc/adc.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

unsigned int joystick_status = DISABLED;

/* from funct_elevator.c */
extern unsigned int elevator_status;

void RIT_IRQHandler (void)
{				
	/*****************************************************
	** actions to do with respect to the elevator status
	*****************************************************/
	switch(elevator_status) {
		case FREE:
			check_button(KEY_1);
			check_button(KEY_2);
			break;
		
		case READY:	
			enable_reservation_timer();
			check_button(INT_0);
			break;
		
		case MOVING:
			disable_alarm_timer();
			check_elevator_arrived();
			check_button(INT_0);
			break;
			
		case REACHING_USER: 
			check_elevator_arrived();
			break;
			
		case STOPPED: 
			enable_alarm_timer();
			check_button(INT_0);
			break;
			
		case EMERGENCY:
			check_button(INT_0);
			check_button(KEY_1);
			check_button(KEY_2);
			break;
		
		case MAINTENANCE:
			ADC_start_conversion();
			break;
			
		default:
			break;
	}
	
	switch(joystick_status) { 
		case SELECT_ENABLED: 
			check_joystick(JOYSTICK_SELECT);
			break; 
		 
		case MOVE_ENABLED:
			check_joystick(JOYSTICK_MOVE);
			break; 
		 
		case DISABLED: 
			/* nothing to do */ 
			break;
		
		default:
			break;
	}
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
