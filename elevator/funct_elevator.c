#include "elevator.h"
#include "lpc17xx.h"
#include "../led/led.h"
#include "../timer/timer.h"

unsigned int elevator_position = GROUND_FLOOR;
unsigned int elevator_status = FREE;
unsigned int request_floor = 0;
unsigned int timer_blinking = DISABLED;

extern unsigned int joystick_status;
extern unsigned int leds_status[8];
extern unsigned int timer_alarm;

void elevator_up() {
	if(elevator_position < FIRST_FLOOR) {
		elevator_status = MOVING;
		elevator_position++;
		if(!timer_blinking) { /* start blinking */
			timer_blinking = ENABLED;
			init_timer(1, HZ_2);
			enable_timer(1);
		}
	}
}

void elevator_down() {
	if(elevator_position > GROUND_FLOOR) {
		elevator_status = MOVING;
		elevator_position--;
		if(!timer_blinking) { /* start blinking */
			timer_blinking = ENABLED;
			init_timer(1, HZ_2);
			enable_timer(1);
		}
	}
}

void stop_elevator() {
	elevator_status = STOPPED;
	if(timer_blinking) { /* stop blinking */
		timer_blinking = DISABLED;
		LED_On(STATUS_LED);
		/* DISABLE TIMER FOR BLINKING */
		clear_timer(1);
	}
}

void free_elevator() {
	LED_Off(RESERVE_LED_0);
	LED_Off(RESERVE_LED_1);
	LED_Off(STATUS_LED);
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);
	elevator_status = FREE;
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
		elevator_status = MOVING;
		
		/* do the first movement */
		if(request_floor == FIRST_FLOOR)
			elevator_up();
		else if(request_floor == GROUND_FLOOR)
			elevator_down();
		
		/* ENABLE TIMER FOR BLINKING */
		init_timer(1, HZ_2);
		enable_timer(1);
	}
}

