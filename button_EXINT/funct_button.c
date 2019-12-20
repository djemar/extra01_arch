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
#include "button.h"
#include "../led/led.h"
#include "../const.h"

extern unsigned int elevator_status;
extern unsigned int elevator_position;
extern unsigned int joystick_select_enabled;
extern unsigned int joystick_move_enabled;

/*----------------------------------------------------------------------------
  Function that turns on requested led
 *----------------------------------------------------------------------------*/
void call_elevator(unsigned int user_floor) {
  NVIC_DisableIRQ(EINT1_IRQn);
  NVIC_DisableIRQ(EINT2_IRQn);
  elevator_status = BUSY;
	joystick_select_enabled = TRUE;
	
  LED_On(0);
  LED_On(2);

  if(user_floor != elevator_position){
    //TODO goto_user();
  }
}

