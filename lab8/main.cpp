// Lab8.cpp
// Runs on LM4F120 or TM4C123
// Student names: Shreyas Kudari and Rakesh Johny
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 3/28/2018 

// Specifications:
// Measure distance using slide pot, sample at 60 Hz
// maximum distance can be any value from 1.5 to 2cm
// minimum distance is 0 cm
// Calculate distance in fixed point, 0.001cm
// Analog Input connected to PD2=ADC5
// displays distance on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats (use them in creative ways)
// must include at least one class used in an appropriate way

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "TExaS.h"
#include "PLL.h"
#include "SlidePot.h"
#include "print.h"

SlidePot Sensor(2000,0);

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF4       (*((volatile uint32_t *)0x40025040))
// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(unsigned long period){
  //*** students write this ******
	NVIC_ST_CTRL_R=0;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_RELOAD_R=period;
	NVIC_SYS_PRI3_R=(NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
	NVIC_ST_CTRL_R=0x07;
}

// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
  //*** students write this ******
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R|=0x10;
	delay++;
	delay++;
	GPIO_PORTF_LOCK_R=GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |=0x0E;				//enable commit for PF1, PF2 and PF3
	GPIO_PORTF_AMSEL_R &= 0xF1;	//disable analog functionality on PF1, PF2 and PF
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;//configure PF0 and PF4 as GPIO
	GPIO_PORTF_AFSEL_R &= 0xF1;	//disable alt funct on PF0 and PF4
	GPIO_PORTF_DIR_R&=0xF1;		
	GPIO_PORTF_DEN_R|=0x0E;
	GPIO_PORTF_PUR_R=0x0E;			
}
uint32_t Data;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.001 cm
int main1(void){      // single step this program and look at Data
  DisableInterrupts();
  TExaS_Init();      // start scope set system clock to 80 MHz
  ADC_Init();        // turn on ADC, PD2, set channel to 5
  EnableInterrupts();
  while(1){                
    Data = ADC_In();  // sample 12-bit channel 5, PD2
  }
}

int main2(void){
  DisableInterrupts();
  TExaS_Init();       // Bus clock is 80 MHz 
  ADC_Init();        // turn on ADC, PD2, set channel to 5
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  EnableInterrupts();
  while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec           
    PF2 = 0x04;       // Profile ADC
    Data = ADC_In();  // sample 12-bit channel 5, PD2
    PF2 = 0x00;       // end of ADC Profile
    ST7735_SetCursor(0,0);
    PF1 = 0x02;       // Profile LCD
    LCD_OutDec(Data); 
    ST7735_OutString("    ");  // these spaces are used to coverup characters from last output
    PF1 = 0;          // end of LCD Profile
  }
}



int main3(void){ 
  DisableInterrupts();
  TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  ADC_Init();        // turn on ADC, PD2, set channel to 5
  EnableInterrupts();
  while(1){  
    PF2 ^= 0x04;      // Heartbeat
    Data = ADC_In();  // sample 12-bit channel 5, PD2
    PF3 = 0x08;       // Profile Convert
    Position = Sensor.Convert(Data); 
    PF3 = 0;          // end of Convert Profile
    PF1 = 0x02;       // Profile LCD
    ST7735_SetCursor(0,0);
    LCD_OutDec(Data); ST7735_OutString("    "); 
    ST7735_SetCursor(6,0);
    LCD_OutFix(Position);
    PF1 = 0;          // end of LCD Profile
  }
}   


// final main program to create distance meter
int main(void){ 
    //*** students write this ******

  DisableInterrupts();
  TExaS_Init();    // bus clock at 80 MHz
  ST7735_InitR(INITR_REDTAB); 
  ADC_Init();        // turn on ADC, PD2, set channel to 5
  PortF_Init();

  // more initializations
  EnableInterrupts();

  while(1){
    Sensor.Sync(); // wait for semaphore
    // can call Sensor.ADCsample, Sensor.Distance, Sensor.Convert as needed 
		uint32_t data = Sensor.Distance();
		LCD_OutFix(data);						//outputs fixed point data to the LCD
		ST7735_OutString("cms");		//prints out the unit
  }
}
void SysTick_Handler(void){ // every 16.67 ms
    //*** students write this ******
// should call ADC_In() and my.Save
	PF1 ^= 0x01;				//togge LED1 for heartbeat
	PF2 ^= 0x01;				//toggle LED2 for heartbeat
	Sensor.Save(ADC_In());
	PF3 ^= 0x01;				//toggle LED2 for heartbeat
}
