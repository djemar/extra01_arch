#include "elevator.h"
#include "lpc17xx.h"
#include "../led/led.h"

unsigned int elevator_position = DOWNSTAIRS;
unsigned int elevator_status = FREE; // BUSY, FREE, ARRIVED, MOVING

extern unsigned int joystick_select_enabled;
extern unsigned int joystick_move_enabled;

void elevator_up() {
	elevator_position++;
	LED_blink(STATUS_LED, HZ_2);
	if(elevator_position == UPSTAIRS) 
		elevator_arrived();
}

void elevator_down() {
	elevator_position--;
	LED_blink(STATUS_LED, HZ_2);
	if(elevator_position == DOWNSTAIRS)
		elevator_arrived();
}

void elevator_arrived() {
  LED_Off(RESERVE_LED_0);
	LED_Off(RESERVE_LED_1);
  elevator_status = ARRIVED;
	joystick_move_enabled = FALSE;
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);
}

void elevator_pause() {
	LED_On(STATUS_LED);
	// TODO  alarm
}

void call_elevator(unsigned int user_floor) {
	NVIC_DisableIRQ(EINT1_IRQn);
  NVIC_DisableIRQ(EINT2_IRQn);

  elevator_status = BUSY;

  LED_On(RESERVE_LED_0);
  LED_On(RESERVE_LED_1);

	if(user_floor == elevator_position) {
		joystick_select_enabled = TRUE;
	} else {
		//TODO goto_user();
	}
}
