// put implementations for functions, explain how it works
// put your names here, date
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "DAC.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	volatile uint32_t delay=0;
	SYSCTL_RCGCGPIO_R|=0x02;
	delay++;
	GPIO_PORTB_DIR_R|=0x3F;
	GPIO_PORTB_AFSEL_R &= ~0x3F; 		//disable alt functions
	GPIO_PORTB_DEN_R|=0x3F;	
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Input=n is converted to n*3.3V/15
// Output: none
void DAC_Out(uint32_t data){

	GPIO_PORTB_DATA_R=data;
	
}
