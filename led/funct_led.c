/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_led.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        High level led management functions
** Correlated files:    lib_led.c, funct_led.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "led.h"
#include "../elevator/elevator.h"
#include "../const.h"

#define LED_NUM     8                   /* Number of user LEDs                */
const unsigned long led_mask[] = { 1UL<<0, 1UL<<1, 1UL<<2, 1UL<< 3, 1UL<< 4, 1UL<< 5, 1UL<< 6, 1UL<< 7 };

extern unsigned char led_value;

unsigned int leds_status[8] = { OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF};
extern unsigned int elevator_status;

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On(unsigned int num) {
	leds_status[num] = ON;
  LPC_GPIO2->FIOPIN |= led_mask[num];
	led_value = LPC_GPIO2->FIOPIN;
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off(unsigned int num) {
  leds_status[num] = OFF;
  LPC_GPIO2->FIOPIN &= ~led_mask[num];
	led_value = LPC_GPIO2->FIOPIN;
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
	led_value = value;
}

void LED_blink(unsigned int num, unsigned int freq){
  static unsigned int blink_counter;
  
  if(elevator_status == ARRIVED) {
   static unsigned int time_counter = 0;
   if(time_counter == 0) blink_counter = 0;
   time_counter++;
   if(time_counter == SEC_3) {
      elevator_status = FREE;
      time_counter = 0;
      blink_counter = 0;
      return;
   }
  }

  blink_counter++;
  if(blink_counter == freq) {
    blink_counter = 0;
    if(leds_status[num] == OFF) 
      LED_On(num);
    else 
      LED_Off(num);
  }

}
