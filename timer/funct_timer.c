#include "timer.h"
#include "lpc17xx.h"
#include "../led/led.h"
#include "../const.h"


void clear_alarm(uint8_t timer_num)
{
    disable_timer(0); 
    reset_timer(0);
	LED_Off(ALARM_LED_0);
    LED_Off(ALARM_LED_1);
}
