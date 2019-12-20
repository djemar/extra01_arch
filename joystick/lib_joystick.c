/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {

	// enable joystick - SELECT
  LPC_PINCON->PINSEL3 &= ~(3<<18);	//PIN mode GPIO (00b value per P1.25 - select)
	LPC_GPIO1->FIODIR   &= ~(1<<25);	//P1.25 Input (joysticks on PORT1 defined as Input)

  // enable joystick - UP
	LPC_PINCON->PINSEL3	&= ~(3<<26);
	LPC_GPIO1->FIODIR		&= ~(1<<29); //P1.29 Input (joysticks on PORT1 defined as Input)
	
	// enabale joystick - DOWN
	LPC_PINCON->PINSEL3	&= ~(3<<20);
	LPC_GPIO1->FIODIR		&= ~(1<<26); //P1.26 Input (joysticks on PORT1 defined as Input)	
	
}
