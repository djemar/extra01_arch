/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Descriptions:        functions to manage T0, T1, T2 interrupts
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

extern uint8_t timer_reservation;
extern uint8_t elevator_status;
extern uint8_t joystick_status;
extern uint8_t timer_blinking;

extern const int freqs[8];
extern int note_index;
extern int note1_index_tmp;
extern int note1_index; /* saved value */
extern int note2_index_tmp;
extern int note2_index; /* saved value */
extern uint8_t selected_note;
extern int save_enabled;

volatile uint16_t SinTable[45] =
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
** Descriptions:		Timer/Counter 0 interrupt handler, used when the elevator arrives,
**									when user is in emergency or when someone call the elevator and 
**									does not use it (in 1 min).
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
			break;
		case STOPPED:
			elevator_emergency_mode(ENABLE);
		case EMERGENCY:
			clear_timer(2);
			if(i%2==0)
				init_timer(2, freqs[note1_index]);
			else
				init_timer(2, freqs[note2_index]);
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
** Descriptions:		Timer/Counter 1 interrupt handler, used for led blinking
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
** Descriptions:		Timer/Counter 2 interrupt handler, used for display polling
**								(MAINTENANCE MODE )or for DAC convertion (EMERGENCY MODE)
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
			if(display.x > 86 && display.x < 154 &&
					display.y > 126 && display.y < 194) {
				/* enter maintenance mode */
				elevator_status = MAINTENANCE;
				LCD_MaintenanceMode();
			}	
			break;
		case MAINTENANCE:
			
			if(display.x > 0 && display.x < 120 &&
					display.y > 72 && display.y < 120) {
				/* NOTE_1 */
				LCD_SelectNote(NOTE_1);
			} 
			
			if(display.x > 120 && display.x < 240 &&
					display.y > 72 && display.y < 120) {
				/* NOTE_2 */
				LCD_SelectNote(NOTE_2);
			} 			
			
			if(save_enabled &&
					display.x > 0 && display.x < 120 &&
					display.y > 272 && display.y < 320) {
				/* save */
				note1_index = note1_index_tmp;
				note2_index = note2_index_tmp;
				LCD_SelectNote(selected_note);
			} 
	
			if(display.x > 120 && display.x < 240 &&
					display.y > 272 && display.y < 320) {
				/* quit */
				LCD_HomeScreen();
				elevator_status = FREE;
			}	
			break;
			
		case EMERGENCY: 
			val = ASM_volume(SinTable[ticks], 30); /* ASM function call */
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
