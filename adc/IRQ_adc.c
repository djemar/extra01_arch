/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
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

int note;
int note1 = 1263;
int note2 = 1263;
int note1_tmp;
int note2_tmp;

char note_GUI[] = " 440 Hz - A ";
char note1_GUI[] = " 440 Hz - A ";
char note2_GUI[] = " 440 Hz - A ";
char note1_GUI_tmp[] = " 440 Hz - A ";
char note2_GUI_tmp[] = " 440 Hz - A ";

extern uint8_t selected_note;

void ADC_IRQHandler(void) {
  int freq;
	int freq_hz;
  char note;
	
	AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){
		AD_last = AD_current;
		freq = freqs[AD_current*7/0xFFF];	
		freq_hz = freqs_hz[AD_current*7/0xFFF];	
		note = notes[AD_current*7/0xFFF];
		
		sprintf(note_GUI, " %d Hz - %c ", freq_hz, note);

		if(note1_tmp != freq && selected_note == NOTE_1) {
			sprintf(note1_GUI_tmp, " %d Hz - %c ", freq_hz, note);
			note1_tmp = freq;
			GUI_Text(77, 110, (uint8_t *) note1_GUI_tmp, Blue2, White);
			GUI_Text(32, 278, (uint8_t *) " save ", White, Green); /* save enabled */
		} else if(note2_tmp != freq && selected_note == NOTE_2) {
			sprintf(note2_GUI_tmp, " %d Hz - %c ", freq_hz, note);
			note2_tmp = freq;
			GUI_Text(77, 236, (uint8_t *) note2_GUI_tmp, Blue2, White);	
			GUI_Text(32, 278, (uint8_t *) " save ", White, Green); /* save enabled */
		}
	}
}
