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
/* 
* NOTES
*/
volatile const int freqs[8]={1062,1125,1263,1417,1592,1684,1890,2120};
volatile const int freqs_hz[8]={523,494,440,392,349,330,294,262};
volatile const char notes[8]={'C', 'B', 'A', 'G', 'F', 'E', 'D', 'C'};
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

volatile int note_index;
volatile int note1_index_tmp = 2;
volatile int note1_index = 2; /* saved value: default 1263 */
volatile int note2_index_tmp = 2;
volatile int note2_index = 2; /* saved value: default 1263 */

extern uint8_t selected_note;
extern int save_enabled;

void ADC_IRQHandler(void) {
	char GUI_note[MAX_NOTE_LENGTH];
	int freq_hz;
  char note;
	
	AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
	note_index = AD_current*7/0xFFF;
  if(AD_current != AD_last){
		AD_last = AD_current;	
		freq_hz = freqs_hz[note_index];	
		note = notes[note_index];
		
		if(note1_index_tmp != note_index && selected_note == NOTE_1) {
			note1_index_tmp = note_index;
			sprintf(GUI_note, "%d Hz - %c", freq_hz, note);
			GUI_Text(128, 206, (uint8_t *) GUI_note, Black, White);
		} else if(note2_index_tmp != note_index && selected_note == NOTE_2) {
			note2_index_tmp = note_index;
			sprintf(GUI_note, "%d Hz - %c", freq_hz, note);
			GUI_Text(128, 206, (uint8_t *) GUI_note, Black, White);
		}
		
		if(save_enabled == TRUE && note1_index == note1_index_tmp && note2_index == note2_index_tmp) {
			/* disable save button */
			save_enabled = FALSE;
			LCD_DrawRectangle(0,272,MAX_X/2,48, Grey);
			GUI_Text(44, 288, (uint8_t *) "SAVE", Black, Grey);
		} else if(save_enabled == FALSE && (note1_index != note1_index_tmp || note2_index != note2_index_tmp)) {
			/* enable save button */
			save_enabled = TRUE;
			LCD_DrawRectangle(0,272,MAX_X/2,48, Green);
			GUI_Text(44, 288, (uint8_t *) "SAVE", White, Green);
		}
	}
}
