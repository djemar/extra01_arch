#include "lpc17xx.h"
#include "RIT.h"
#include "../elevator/elevator.h"
#include "../button_EXINT/button.h"
#include "../const.h"
#include "../led/led.h"
#include "../timer/timer.h"

volatile uint8_t state_key1=0;
volatile uint8_t state_key2=0;
volatile uint8_t state_int0=0;

/* timers status */
volatile uint8_t timer_alarm = DISABLED;
volatile uint8_t timer_reservation = DISABLED;

extern uint8_t elevator_status;
extern uint8_t joystick_status;

extern uint16_t elevator_position;
extern uint8_t timer_blinking;

void check_button(int button) {
	switch(button) {
		
		case KEY_1:
			/* first floor button pressed */
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ /* read button - pin port 2 --> if(PIN in pos 11 is already pressed) then ... */
				if(state_key1 == 0) {
					state_key1++;
				} else if(state_key1 == 1) {
					call_elevator(FIRST_FLOOR); // first floor (position 288)
					state_key1++;	
				}
			} else /* button released */
				state_key1 = 0;			
			break;
		
		case KEY_2:
			/* ground floor button pressed */
			if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){ /* read button - pin port 2 --> if(PIN in pos 12 is already pressed) then ... */
				if(state_key2 == 0)
					state_key2++;
				else if(state_key2 == 1) {
					call_elevator(GROUND_FLOOR); // ground floor (position 0)
					state_key2++;
				}
			} else /* button released */
				state_key2 = 0;			
			break;
		
		case INT_0:
			/* check emergency */
			if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){ /* read button - pin port 2 --> if(PIN in pos 10 is already pressed) then ... */
				if(state_int0 == 1) { /* if in emergency mode, exit */
					if(elevator_status == EMERGENCY) {
						elevator_emergency_mode(USER_DISABLE);
						state_int0 = 0;
					} else 
						state_int0++;
				} else if(state_int0 == 80) {
					if(elevator_status != EMERGENCY) { /* enter emergency mode */
						elevator_emergency_mode(ENABLE);
					}
				} else {
					state_int0++;
				}
			} else {	/* button released */
				state_int0 = 0;			
			}
			break;
		
		default:
			break;
	}
}

void check_joystick(int joystick_button) {
	switch(joystick_button) {
		
		case JOYSTICK_SELECT:
			/* Joytick Select pressed */ 
			if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){				
				joystick_status = MOVE_ENABLED; 
				LED_On(STATUS_LED); 
				disable_reservation_timer();
				elevator_status = STOPPED; 
			} 
			break;
			
		case JOYSTICK_MOVE:
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){ /* Joytick Up pressed */ 
				if(elevator_up()) {
					elevator_status = MOVING;
				}
			} else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){ /* Joytick Down pressed */
				if(elevator_down()) {
					elevator_status = MOVING;			
				}		 
			} else { /* Joytick Down & Up released */ 
				stop_elevator(); 
			} 
			break;
			
		default:
			break;
	}
}

void enable_reservation_timer() {
	if(timer_reservation == DISABLED){
		clear_timer(0);
		init_timer(0, MIN_1);
		enable_timer(0);
		timer_reservation = ENABLED;
	}
}

void disable_reservation_timer() {
	if(timer_reservation == ENABLED){ 
		clear_timer(0); 
		timer_reservation = DISABLED; 
	} 
}

void enable_alarm_timer() {
	if(timer_alarm == DISABLED){
		clear_timer(0);
		init_timer(0, MIN_1);
		enable_timer(0);
		timer_alarm = ENABLED;
	}
}

void disable_alarm_timer() {
	/* disable alarm timer */
	if(timer_alarm == ENABLED){
		clear_timer(0);
		timer_alarm = DISABLED;
	}
}

void enable_timer_3s() {
	/* ENABLE TIMER FOR 3s BLINKING */
	timer_blinking = ENABLED;		
	clear_timer(0);
	init_timer(0, SEC_3);
	enable_timer(0);
	clear_timer(1);
	init_timer(1, HZ_5);
	enable_timer(1);
}

void check_elevator_arrived() { /* check if the elevator is arrived */
	if(elevator_position == FIRST_FLOOR || elevator_position == GROUND_FLOOR) {
		if(elevator_status == MOVING) { /* the user has reached the floor */
			elevator_status = ARRIVED;
			joystick_status = DISABLED;
		} else if(elevator_status == REACHING_USER) { /* elevator has reached the user */
			elevator_status = USER_REACHED;
		}		
		enable_timer_3s();
		} else if(elevator_status == REACHING_USER) /* the elevator is not arrived and it is reaching user */ 
			move_elevator();
}
