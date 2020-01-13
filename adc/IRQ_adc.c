/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "stdio.h"
#include "adc.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../const.h"
#include "../GLCD/GLCD.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */

//const int freqs[8]={4240,3779,3367,3175,2834,2525,2249,2120};
/* 
131Hz		k=4240 C3
147Hz		k=3779
165Hz		k=3367
175Hz		k=3175
196Hz		k=2834		
220Hz		k=2525
247Hz		k=2249
262Hz		k=2120 C4
*/

const int freqs[8]={1062,1125,1263,1417,1592,1684,1890,2120};
const int freqs_hz[8]={523,494,440,392,349,330,294,262};
const char notes[8]={'C', 'B', 'A', 'G', 'F', 'E', 'D', 'C'};
/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5

*/

int freq;
int note1_freq = 1263;
int note2_freq = 1263;
int note1_freq_tmp;
int note2_freq_tmp;
int freq_hz;
char note;

char note1_GUI[] = " 440 Hz - A ";
char note2_GUI[] = " 440 Hz - A ";
char note1_GUI_tmp[] = " 440 Hz - A ";
char note2_GUI_tmp[] = " 440 Hz - A ";


extern unsigned int note1_status;
extern unsigned int note2_status;

void ADC_IRQHandler(void) {
  	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){
		freq = freqs[AD_current*7/0xFFF];	
		freq_hz = freqs_hz[AD_current*7/0xFFF];	
		note = notes[AD_current*7/0xFFF];
		AD_last = AD_current;
		
		if(note1_status == ENABLED) {
			sprintf(note1_GUI_tmp, " %d Hz - %c ", freq_hz, note);
			note1_freq_tmp = freq;
			GUI_Text(77, 110, (uint8_t *) note1_GUI_tmp, Blue2, White);
			GUI_Text(32, 278, (uint8_t *) " save ", White, Green); /* save enabled */
		} else if(note2_status == ENABLED) {
			sprintf(note2_GUI_tmp, " %d Hz - %c ", freq_hz, note);
			note2_freq_tmp = freq;
			GUI_Text(77, 236, (uint8_t *) note2_GUI_tmp, Blue2, White);	
			GUI_Text(32, 278, (uint8_t *) " save ", White, Green); /* save enabled */
		}

	}
	
}
