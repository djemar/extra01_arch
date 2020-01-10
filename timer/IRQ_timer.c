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
#include "../dac/dac.h"

extern unsigned int timer_reservation;
extern unsigned int elevator_status;
extern unsigned int joystick_status;
extern unsigned int timer_blinking;

uint16_t SinTable[45] =
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


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
	static int i=0;	
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
		case EMERGENCY:
			reset_timer(2);
			disable_timer(2);
			if(i%2==0)
				init_timer(2, 2120);
			else
				init_timer(2, 1062);
			i++;
			enable_timer(2);
			LED_blink(STATUS_LED);
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
	static int ticks=0;
	int val;
	
	getDisplayPoint(&display, Read_Ads7846(), &matrix );
	
	switch(elevator_status) {
		case FREE:
			if(display.x > 48 && display.x < 192 &&
					display.y > 160 && display.y < 176) {
				/* enter maintenance mode */
				elevator_status = MAINTENANCE;
				LCD_MaintenanceMode();
			}	
			break;
		case MAINTENANCE:
			
			if(display.x > 32 && display.x < 208 &&
					display.y > 58 && display.y < 136) {
				/* NOTE_1 */
				LCD_MaintenanceModeSelection(NOTE_1);
			} 
			
			if(display.x > 32 && display.x < 208 &&
					display.y > 184 && display.y < 262) {
				/* NOTE_1 */
				LCD_MaintenanceModeSelection(NOTE_2);
			} 			
			
			if(display.x > 32 && display.x < 80 &&
					display.y > 273 && display.y < 299) {
				/* save */
			} 
	
			if(display.x > 160 && display.x < 208 &&
					display.y > 273 && display.y < 299) {
				/* quit */
				LCD_HomeScreen();
				elevator_status = FREE;
			}	
			break;
			
		case EMERGENCY: 
			/* DAC management */	
			// TODO int val = SinTable[ticks] * 1228 / 819; /* 4095 : 100 = x : 30; 1228 : 819 = x : 410 */
			val = SinTable[ticks];
			DAC_convert (val<<6);
			ticks++;
			if(ticks==45) ticks=0;
		default:
			break;
	}
	
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
