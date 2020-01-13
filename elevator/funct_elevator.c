#include "elevator.h"
#include "lpc17xx.h"
#include "../led/led.h"
#include "../timer/timer.h"

unsigned int elevator_position = GROUND_FLOOR;
unsigned int elevator_status = FREE;
unsigned int request_floor = 0;
unsigned int timer_blinking = DISABLED;

extern unsigned int joystick_status;
extern unsigned int timer_alarm;

int elevator_up() {
	if(elevator_position < FIRST_FLOOR) {
		elevator_position++;
		if(!timer_blinking) { /* start blinking */
			timer_blinking = ENABLED;
			init_timer(1, HZ_2);
			enable_timer(1);
		}
		return 1;
	}
	return 0;
}

int elevator_down() {
	if(elevator_position > GROUND_FLOOR) {
		elevator_position--;
		if(!timer_blinking) { /* start blinking */
			timer_blinking = ENABLED;
			init_timer(1, HZ_2);
			enable_timer(1);
		}
		return 1;
	}
	return 0;
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

void move_elevator() {
	/* function used when the elevator is reaching the user */
	if(request_floor == FIRST_FLOOR)
		elevator_up();
	else if(request_floor == GROUND_FLOOR)
		elevator_down();
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
		if(elevator_status == EMERGENCY) {
			elevator_emergency_mode(RESCUE_DISABLE);
		}
		request_floor = user_floor;
		elevator_status = REACHING_USER;
		
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

void elevator_emergency_mode(int status) {
	if(status == ENABLE) {
		elevator_status = EMERGENCY;
		joystick_status = DISABLED;
		LED_On(ALARM_LED_0);
		LED_On(ALARM_LED_1);
		
		clear_timer(0);
		clear_timer(1);
		init_timer(0, HZ_4);
		enable_timer(0);
	} else {
		clear_timer(0);
		clear_timer(2);
		LED_Off(ALARM_LED_0);
		LED_Off(ALARM_LED_1);
		if(status == USER_DISABLE) {
			elevator_status = STOPPED;
			joystick_status = MOVE_ENABLED;
			LED_On(STATUS_LED);
		} else if(status == RESCUE_DISABLE) {
			elevator_status = REACHING_USER;
			joystick_status = DISABLED;
		}
	}
}

