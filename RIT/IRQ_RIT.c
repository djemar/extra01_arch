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

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

int state_key1=0;
int state_key2=0;
unsigned int joystick_status = DISABLED;
unsigned int timer_alarm = DISABLED;

/* from funct_elevator.c */
extern unsigned int elevator_position;
extern unsigned int elevator_status;
extern unsigned int elevator_old_status;

/* from funct_led.c */
extern unsigned int blink_counter;

void RIT_IRQHandler (void)
{				
	/*****************************************************
	** actions to do with respect to the elevator status
	*****************************************************/
	switch(elevator_status) { // FREE, REACHING_USER, BUSY, STOPPED, ARRIVED
		case FREE:
			/* first floor button pressed */
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ /* read button - pin port 2 --> if(PIN in pos 11 is already pressed) then ... */
				switch(state_key1){
					case 0:
						state_key1++;
						break;
					case 1:
						call_elevator(UPSTAIRS); // first floor (position 288)
						state_key1++;
						break;
					default:
						break;
				}
			} else {	/* button released */
				state_key1 = 0;			
			}
			/* ground floor button pressed */
			if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){ /* read button - pin port 2 --> if(PIN in pos 11 is already pressed) then ... */
				switch(state_key2){
					case 0:
						state_key2++;
						break;
					case 1:
						call_elevator(DOWNSTAIRS); // ground floor (position 0)
						state_key2++;
						break;
					default:
						break;
				}
			} else {	/* button released */
				state_key2 = 0;			
			}
			break;
		
		case READY:
			break;
		
		case REACHING_USER:
			elevator_reach_user();
			break;
		
		case MOVING:
			if(timer_alarm == ENABLED){
				clear_alarm(0);
				timer_alarm = DISABLED;
			}
			
			break;
			
		case STOPPED: 
			blink_counter = 0; // why?
			/* TODO timer alarm */
			if(timer_alarm == DISABLED){
				init_timer(0, MIN_1);
				enable_timer(0);
				timer_alarm = ENABLED;
			}
			break;
		
		case ARRIVED: 
			elevator_arrived();
			break;
		
		default:
			break;
	}
	
	switch(joystick_status) {
		case SELECT_ENABLED:
			/* Joytick Select pressed */
			if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){ 
				joystick_status = MOVE_ENABLED;
				LED_On(STATUS_LED);
			}
			break;
		
		case MOVE_ENABLED:
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){ /* Joytick Up pressed */
				if(elevator_position != UPSTAIRS) {
					elevator_status = MOVING;
					elevator_up();
				}
			} else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){ /* Joytick Down pressed */
				if(elevator_position != DOWNSTAIRS) {
					elevator_status = MOVING;
					elevator_down();
				}
			} else { /* Joytick Down & Up released */ 
				elevator_status = STOPPED;
				LED_On(STATUS_LED);
			}
			break;
		
		case DISABLED:
			/* nothing to do */
			break;
		
	}
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
