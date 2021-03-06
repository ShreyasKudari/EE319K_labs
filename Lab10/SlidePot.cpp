// SlidePot.cpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/28/2018 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "SlidePot.h"
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){ 
// lab 8
	volatile int delay=0;
	SYSCTL_RCGCGPIO_R |= 0x08;	//activate port D clock
	delay++;
	GPIO_PORTD_DIR_R|= ~0x04;
	GPIO_PORTD_AFSEL_R |= 0x04;
	GPIO_PORTD_DEN_R &= ~0x04;
	GPIO_PORTD_AMSEL_R |= 0x04;
	SYSCTL_RCGCADC_R|=0x01;			//enable ADC clock
	delay++;
	delay++;
	delay++;
	delay++;			//extra time to stabilize
	ADC0_PC_R=0x01;		//sampling speed 125ksamples/second
	ADC0_SSPRI_R=0x0123;	//set priorities for the sequencers. Sequencer 3 is the highest priority
	ADC0_ACTSS_R &= ~0x0008; //disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;		//seq 3 is software trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+5;	//Ain5
	ADC0_SSCTL3_R = 0x0006;	
	ADC0_IM_R &= ~0x0008;	//disable 	SS3 interrupts
	ADC0_ACTSS_R |= 0x0008;	//enable sample sequencer 3
	ADC0_SAC_R = 0x06;     //hardware averaging for stability
	
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  

// lab 8
	uint32_t data;
	ADC0_PSSI_R=0x0008;		//start ADC

	while((ADC0_RIS_R&0x08)==0){}	//busy wait 
		data=ADC0_SSFIFO3_R&0xFFF;	//read bits 11-0 
		ADC0_ISC_R=0x0008;		//clear flag
		return data; // remove this, replace with real code
  return 0; // remove this, replace with real code
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
SlidePot::SlidePot(uint32_t m, uint32_t b){
// lab 8
	data=0;		
	flag=0;
	distance=0;
	slope=m;
	offset=b;

	
}

void SlidePot::Save(uint32_t n){
// lab 9
	data=n;
	distance=Convert(n);
	flag=1;
}
uint32_t SlidePot::Convert(uint32_t n){
  // lab 8
	return (1556*n)/4096+311;
  
}

void SlidePot::Sync(void){
// lab 8
	// 1) wait for semaphore flag to be nonzero
// 2) set semaphore flag to 0
	while(flag==0)
	{}
		flag=0;
}

uint32_t SlidePot::ADCsample(void){ // return ADC sample value (0 to 4095)
	 return data;
}

uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
// lab 8
	
  return distance;
}


