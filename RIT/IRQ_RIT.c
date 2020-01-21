/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Descriptions:        functions to manage bouncing and elevator status
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../const.h"
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

volatile uint8_t joystick_status = DISABLED;

/* from funct_elevator.c */
extern uint8_t elevator_status;

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
		
		default: /* case DISABLED */
			/* nothing to do */ 
			break;
	}
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
