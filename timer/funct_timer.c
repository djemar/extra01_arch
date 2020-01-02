#include "timer.h"
#include "lpc17xx.h"
#include "../led/led.h"
#include "../const.h"


void clear_timer(uint8_t timer_num)
{
    disable_timer(timer_num); 
    reset_timer(timer_num);
    if(timer_num == 0){
        LED_Off(ALARM_LED_0);
        LED_Off(ALARM_LED_1);
    }
}
