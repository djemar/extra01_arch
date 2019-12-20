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
#include "../joystick/joystick.h"
#include "../button_EXINT/button.h"
#include "../const.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int state_key1=0;
volatile int state_key2=0;
volatile unsigned int joystick_select_enabled = FALSE;
volatile unsigned int joystick_move_enabled = FALSE;

volatile unsigned int elevator_status = FREE;
volatile unsigned int elevator_position = DOWNSTAIRS; /* 4 km/h = 1.11111 m/s
																	   * 8 m -> 8/1.11111 = 7.2 s = 7200 ms
																		 * polling: 25 ms --> 7200/25 = 288
																		 * elevator position [0, 288] = [DOWNSTAIRS, UPSTAIRS]
																		 */ 

void RIT_IRQHandler (void)
{				

	/********************* 
	** joystick management
	*********************/

	if(joystick_select_enabled == TRUE) {
		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){
		/* Joytick Select pressed */
		joystick_select_enabled = FALSE;
		joystick_move_enabled = TRUE;
		}
	}

	if(joystick_move_enabled) {
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
			/* Joytick Up pressed */
			elevator_up();
		}
	
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
			/* Joytick Down pressed */
			elevator_down();
		}
	}
	
	/******************* 
	** button management
	*******************/

		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ /* read button - pin port 2 --> if(PIN in pos 11 is already pressed) then ... */
			switch(state_key1){
				case 0:
					state_key1++;
					break;
				case 1:
					call_elevator(DOWNSTAIRS); // first floor (position 0)
					state_key1++;
					break;
				default:
					break;
			}
		} else {	/* button released */
			state_key1 = 0;			
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	
		
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){ /* read button - pin port 2 --> if(PIN in pos 11 is already pressed) then ... */
			switch(state_key2){
				case 0:
					state_key2++;
					break;
				case 1:
					call_elevator(UPSTAIRS); // second floor (position 288)
					state_key2++;
					break;
				default:
					break;
			}
		} else {	/* button released */
			state_key2 = 0;			
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}

  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */

  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
