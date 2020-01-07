/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "../const.h"
#include "../led/led.h"
#include "../elevator/elevator.h"
#include "timer.h"


extern unsigned int timer_reservation;
extern unsigned int elevator_status;


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
  switch(elevator_status){
    case STOPPED:
      LED_On(ALARM_LED_0);
      LED_On(ALARM_LED_1);
      break;
    case READY:
      clear_timer(1); 
	    free_elevator();
      timer_reservation = DISABLED;
      break;
    case REACHING_USER:
      elevator_old_status = elevator_status; /* it can be REACHING_USER or MOVING */
	  	elevator_status = ARRIVED;
		  joystick_status = DISABLED;
		  blink_counter = 0;
		  elevator_arrived();
    case default:
    break;
  }
  
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
