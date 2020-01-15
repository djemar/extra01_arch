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

#include "adc/adc.h"
#include "dac/dac.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	
	LCD_Initialization();
	TP_Init();
	TouchPanel_Calibrate();	
	LCD_HomeScreen(); /* draw home screen */
	/* timer for touch screen input */
	init_timer(2, 0x4E2); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	enable_timer(2);
	
	LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40>>1);							/* RIT Initialization 50/2 = 25 msec       	*/
	enable_RIT();													/* RIT enabled												*/
	
	ADC_init();														/* ADC Initialization									*/
	DAC_init();														/* DAC Initialization	  							*/
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
