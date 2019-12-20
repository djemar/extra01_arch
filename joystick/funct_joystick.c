/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "joystick.h"
#include "../led/led.h"
#include "../const.h"

extern unsigned int elevator_position;
	
extern unsigned int joystick_select_enabled;
extern unsigned int joystick_move_enabled;

/*----------------------------------------------------------------------------
  Function that turns on requested led
 *----------------------------------------------------------------------------*/

void elevator_up() {
  elevator_position++;
  if(elevator_position == UPSTAIRS) {
    LED_Off(0);
    LED_Off(2);
		joystick_move_enabled = FALSE;
		//joystick_select_enabled = FALSE;
		NVIC_EnableIRQ(EINT1_IRQn);
		NVIC_EnableIRQ(EINT2_IRQn);
  }
}
void elevator_down() {
  elevator_position--;
	if(elevator_position == DOWNSTAIRS) {
		LED_Off(0);
    LED_Off(2);
		joystick_move_enabled = FALSE;
		//joystick_select_enabled = FALSE;
		NVIC_EnableIRQ(EINT1_IRQn);
		NVIC_EnableIRQ(EINT2_IRQn);
  }
}
