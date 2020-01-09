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
unsigned int timer_reservation = DISABLED;

/* from funct_elevator.c */
extern unsigned int elevator_position;
extern unsigned int elevator_status;
extern unsigned int timer_blinking;

void RIT_IRQHandler (void)
{				
	/*****************************************************
	** actions to do with respect to the elevator status
	*****************************************************/
	switch(elevator_status) {
		case FREE:
			/* first floor button pressed */
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ /* read button - pin port 2 --> if(PIN in pos 11 is already pressed) then ... */
				switch(state_key1){
					case 0:
						state_key1++;
						break;
					case 1:
						call_elevator(FIRST_FLOOR); // first floor (position 288)
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
						call_elevator(GROUND_FLOOR); // ground floor (position 0)
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
			if(timer_reservation == DISABLED){
				init_timer(0, MIN_1);
				enable_timer(0);
				timer_reservation = ENABLED;
			}
			break;
		
		case MOVING:
			/* disable alarm timer */
			if(timer_alarm == ENABLED){
				clear_timer(0);
				timer_alarm = DISABLED;
				LED_Off(ALARM_LED_0);
   				LED_Off(ALARM_LED_1);
			}
			/* check if the elevator is arrived */
			if(elevator_position == FIRST_FLOOR || elevator_position == GROUND_FLOOR) {
				if(joystick_status == DISABLED) { /* elevator has reached the user */
					elevator_status = USER_REACHED;
				} else { /* the user has reached the floor */
					elevator_status = ARRIVED;
					joystick_status = DISABLED;
				}
		
				/* ENABLE TIMER FOR 3s BLINKING */
				timer_blinking = ENABLED;		
				clear_timer(0);
				init_timer(0, SEC_3);
				enable_timer(0);
				clear_timer(1);
				init_timer(1, HZ_5);
				enable_timer(1);
				
			} else if(joystick_status == DISABLED) { /* check if the elevator is not arrived and not controlled by the user */ 
				/* then the elevator reaches the user */
				move_elevator();
			}
			break;
			
		case STOPPED: 
			if(timer_alarm == DISABLED){
				init_timer(0, MIN_1);
				enable_timer(0);
				timer_alarm = ENABLED;
			}
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
				if(timer_reservation == ENABLED){
					clear_timer(0);
					timer_reservation = DISABLED;
				}
				elevator_status = STOPPED;
			}
			
			break;
		
		case MOVE_ENABLED:
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){ /* Joytick Up pressed */
				elevator_up();
			} else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){ /* Joytick Down pressed */
				elevator_down();	
			} else { /* Joytick Down & Up released */
				stop_elevator();
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
