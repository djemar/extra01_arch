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
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"

extern unsigned int timer_reservation;
extern unsigned int elevator_status;
extern unsigned int joystick_status;
extern unsigned int timer_blinking;

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
      clear_timer(0); 
	    free_elevator();
      timer_reservation = DISABLED;
      break;
		case ARRIVED:
			clear_timer(0);
			clear_timer(1);
			timer_blinking = DISABLED;
			free_elevator();
			break;
		case USER_REACHED:
			clear_timer(0);
			clear_timer(1);
			elevator_status = READY;
			joystick_status = SELECT_ENABLED;
			timer_blinking = DISABLED;
			break;
    default:
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
	LED_blink(STATUS_LED);
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
	getDisplayPoint(&display, Read_Ads7846(), &matrix );
	
	if(display.x <= 240 && display.x <= 0) {
		int i = 0;
		i = i + 1;
	}
	
	if(display.x > 32 && display.x < 80 &&
			display.y > 278 && display.y < 294) {
		/* save */
				int i = 0;
				i = i + 1;
	} 
	
	if(display.x > 160 && display.x < 208 &&
			display.y > 278 && display.y < 294) {
		/* quit */
				int i = 0;
				i = i + 1;
	}
	
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
