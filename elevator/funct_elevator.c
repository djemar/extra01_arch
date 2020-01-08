#include "elevator.h"
#include "lpc17xx.h"
#include "../led/led.h"

unsigned int elevator_position = DOWNSTAIRS;
unsigned int elevator_status = FREE;
unsigned int elevator_old_status = FREE;
unsigned int request_floor = 0;
unsigned int time_counter = 0;

extern unsigned int joystick_status;
extern unsigned int blink_counter;
extern unsigned int leds_status[8];

void elevator_reach_user() {
	if(request_floor > elevator_position) 
		elevator_up();
	else
		elevator_down();
}

void elevator_up() {
	elevator_position++;
	LED_blink(STATUS_LED, HZ_2);
		
	if(elevator_position == UPSTAIRS) {
		elevator_old_status = elevator_status; /* it can be REACHING_USER or MOVING */
		elevator_status = ARRIVED;
		joystick_status = DISABLED;
		blink_counter = 0;
		
		elevator_arrived();
	}
}
	

void elevator_down() {
	elevator_position--;
	LED_blink(STATUS_LED, HZ_2);
	
	if(elevator_position == DOWNSTAIRS) {
		elevator_old_status = elevator_status; /* it can be REACHING_USER or MOVING */
		elevator_status = ARRIVED;
		joystick_status = DISABLED;
		blink_counter = 0;
		
		elevator_arrived();
	}
}

void elevator_arrived() {
	time_counter++;
	LED_blink(STATUS_LED, HZ_5);
	
	if(time_counter == SEC_3) {
		time_counter = 0;
		blink_counter = 0;
		switch(elevator_old_status) {
			case REACHING_USER:
				elevator_status = READY;
				joystick_status = SELECT_ENABLED;
				break;
			case MOVING:
				free_elevator();
				break;
			default:
				break;
		}
	}
}

void free_elevator() {
	LED_Off(RESERVE_LED_0);
	LED_Off(RESERVE_LED_1);
	LED_Off(STATUS_LED);
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);
	elevator_status = FREE;
	joystick_status = DISABLED;
}

void call_elevator(unsigned int user_floor) {
	NVIC_DisableIRQ(EINT1_IRQn);
  	NVIC_DisableIRQ(EINT2_IRQn);

	LED_On(RESERVE_LED_0);
	LED_On(RESERVE_LED_1);

	if(user_floor == elevator_position) {
		joystick_status = SELECT_ENABLED;
		elevator_status = READY;
	} else {
		request_floor = user_floor;
		elevator_status = REACHING_USER;
	}
}

