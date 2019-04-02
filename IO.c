// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: March 30, 2018
// Last Modified:  change this or look silly
// Lab number: 7


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "ST7735.c"

int hb_toggle = 0;

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {
 // --UUU-- Code to initialize PF4 and PF2
	volatile uint32_t delay=0;
	SYSCTL_RCGCGPIO_R|=0x20;												//initializes system clock for PortF
	delay++;
	GPIO_PORTF_DEN_R |= 0x14;
	GPIO_PORTF_DIR_R &= 0xFFFFFFEF;												 //PF4 is switch(input)
	GPIO_PORTF_DIR_R |= 0x4;															//PF2 is output
	
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
 // --UUU-- PF2 is heartbeat
		GPIO_PORTF_DATA_R^=(hb_toggle/100000)<<2;					//heartbeat toggles after 100,000 iterations
		hb_toggle=(hb_toggle+1)%100001;
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
 // --UUU-- wait for release; delay for 20ms; and then wait for press
	int data = GPIO_PORTF_DATA_R &= 0xFFFFFFFB;			//isolates PF2
	while(1){
		if(data == 4)
			break;
	}
	Delay1ms(20);
	data = GPIO_PORTF_DATA_R &= 0xFFFFFFFB;			//isolates PF2
	while(1){
		if(data == 0)
			break;
	}
}  


