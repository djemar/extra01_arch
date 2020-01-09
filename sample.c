/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT and joystick buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"

#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	
	LCD_Initialization();
	TP_Init();
	TouchPanel_Calibrate();	
	LCD_Clear(Black);
	GUI_Text(48, 160, (uint8_t *) " MAINTENANCE MODE ", White, Red);
	
	/*
	LCD_Clear(Blue2);
	GUI_Text(76, 26, (uint8_t *) "Maintenance", Yellow, Blue2);
	GUI_Text(60, 68, (uint8_t *) " Select note 1 ", White, Blue2);
	GUI_Text(0, 110, (uint8_t *) " 440 Hz - A ", White, Blue2);
	LCD_DrawLine(76, 152, 164, 152, White);
	GUI_Text(60, 194, (uint8_t *) " Select note 2 ", White, Blue2);
	GUI_Text(0, 236, (uint8_t *) " 440 Hz - A ", White, Blue2);
	GUI_Text(32, 278, (uint8_t *) " save ", White, Green);
	GUI_Text(160, 278, (uint8_t *) " quit ", White, Red);
	*/
	
	init_timer(2, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	enable_timer(2);
	
	LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40>>1);							/* RIT Initialization 50/2 = 25 msec       	*/
	enable_RIT();													/* RIT enabled												*/
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
