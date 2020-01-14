/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           RIT.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_RIT, funct_RIT, IRQ_RIT .c files
** Correlated files:    lib_RIT.c, funct_RIT.c, IRQ_RIT.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __RIT_H
#define __RIT_H

#include <stdint.h>

/* init_RIT.c */
extern uint32_t init_RIT( uint32_t RITInterval );
extern void enable_RIT( void );
extern void disable_RIT( void );
extern void reset_RIT( void );
/* IRQ_RIT.c */
extern void RIT_IRQHandler (void);
/* funct_RIT.c */
void check_button(int button);
void check_joystick(int joystick_button);
void enable_reservation_timer(void);
void disable_reservation_timer(void);
void disable_alarm_timer(void);
void check_elevator_arrived(void);
void enable_alarm_timer(void);

#endif /* end __RIT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
