// Piano.c
// This software configures the off-board piano keys
// Lab 6 requires a minimum of 3 keys, but you could have more
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/5/18 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// **************Piano_Init*********************
// Initialize piano key inputs, called once to initialize the digital ports
// Input: none 
// Output: none
void Piano_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R|=0x30;
	delay++;
	GPIO_PORTE_DIR_R&=0xF8;
	GPIO_PORTE_DEN_R|=0x07;
	GPIO_PORTF_LOCK_R=GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |=0x11;				//enable commit for PF4 and PF0
	 GPIO_PORTF_AMSEL_R &= 0xEE;	//disable analog functionality on PF4 and PF0
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFF0FFF0)+0x00000000;//configure PF0 and PF4 as GPIO
	GPIO_PORTF_AFSEL_R &= 0xEE;	//disable alt funct on PF0 and PF4
	GPIO_PORTF_DIR_R&=0xEE;			//for song and stop extra credit
	GPIO_PORTF_DEN_R|=0x11;
	GPIO_PORTF_PUR_R=0x11;			//PF4 and PF0 is positive logic
	
	
	

}

// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 7 depending on keys
//   0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
//   bit n is set if key n is pressed
uint32_t Piano_In(void){
	
	return GPIO_PORTE_DATA_R&0x0000000F; // Replace with your code
}
