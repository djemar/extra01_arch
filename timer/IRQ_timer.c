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
#include "string.h"
#include "../const.h"
#include "../led/led.h"
#include "../elevator/elevator.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../dac/dac.h"

extern int ASM_volume(int, int);

extern unsigned int timer_reservation;
extern unsigned int elevator_status;
extern unsigned int joystick_status;
extern unsigned int timer_blinking;

extern unsigned int note1;
extern unsigned int note1_tmp;
extern unsigned int note2;
extern unsigned int note2_tmp;
extern char note1_GUI[];
extern char note2_GUI[];
extern char note1_GUI_tmp[];
extern char note2_GUI_tmp[];


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
			LED_Off(STATUS_LED);
			elevator_status = READY;
			joystick_status = SELECT_ENABLED;
			timer_blinking = DISABLED;
			LED_Off(STATUS_LED);
			break;
		case STOPPED:
			elevator_emergency_mode(ENABLE);
		case EMERGENCY:
			clear_timer(2);
			if(i%2==0)
				init_timer(2, note1);
			else
				init_timer(2, note2);
			i++;
			enable_timer(2);
			LED_blink(STATUS_LED);
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
	static int ticks=0;
	volatile int val;
	
	getDisplayPoint(&display, Read_Ads7846(), &matrix );
	
	switch(elevator_status) {
		case FREE:
			
	LCD_icon(92, 132);
			if(display.x > 92 && display.x < 148 &&
					display.y > 132 && display.y < 188) {
				/* enter maintenance mode */
				elevator_status = MAINTENANCE;
				LCD_MaintenanceMode();
			}	
			break;
		case MAINTENANCE:
			
			if(display.x > 32 && display.x < 208 &&
					display.y > 58 && display.y < 136) {
				/* NOTE_1 */
				LCD_MaintenanceModeSelectNote(NOTE_1);
			} 
			
			if(display.x > 32 && display.x < 208 &&
					display.y > 184 && display.y < 262) {
				/* NOTE_2 */
				LCD_MaintenanceModeSelectNote(NOTE_2);
			} 			
			
			if(display.x > 32 && display.x < 80 &&
					display.y > 273 && display.y < 299) {
				/* save */
				strcpy(note1_GUI, note1_GUI_tmp);
				strcpy(note2_GUI, note2_GUI_tmp);
				note1 = note1_tmp;
				note2 = note2_tmp;
				LCD_MaintenanceMode();
			} 
	
			if(display.x > 160 && display.x < 208 &&
					display.y > 273 && display.y < 299) {
				/* quit */
				LCD_HomeScreen();
				elevator_status = FREE;
				strcpy(note1_GUI_tmp, note1_GUI);
				strcpy(note2_GUI_tmp, note2_GUI);
			}	
			break;
			
		case EMERGENCY: 
			/* DAC management */	
			/* MAX SinTable = 819; MAX 10 bit = 1023 -> 1023 : 100 = 819 : x -> x = 80 % */
			/* SinTable[ticks] : 80 = x : 30 -> val30% = SinTable[ticks] * 30 / 80 */
			val = ASM_volume(SinTable[ticks], 30); /* val = SinTable[ticks] * 30 / 80; */
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
