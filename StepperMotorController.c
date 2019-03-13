// StepperMotorController.c starter file EE319K Lab 5
// Runs on TM4C123
// Finite state machine to operate a stepper motor.  
// Shreyas Kudari/Rakesh Johny
// March 12, 2019

// Hardware connections (External: two input buttons and four outputs to stepper motor)
//  PA5 is Wash input  (1 means pressed, 0 means not pressed)
//  PA4 is Wiper input  (1 means pressed, 0 means not pressed)
//  PE5 is Water pump output (toggle means washing)
//  PE4-0 are stepper motor outputs 
//  PF1 PF2 or PF3 control the LED on Launchpad used as a heartbeat
//  PB6 is LED output (1 activates external LED on protoboard)

#include "SysTick.h"
#include "TExaS.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

void EnableInterrupts(void);
// edit the following only if you need to move pins from PA4, PE3-0      
// logic analyzer on the real board
#define PA4       (*((volatile unsigned long *)0x40004040))
#define PE50      (*((volatile unsigned long *)0x400240FC))
#define HOME			&FSM[0]
#define	Wash1			&FSM[1]
#define Wipe2			&FSM[2]
#define Wash3			&FSM[3]
#define Wipe4			&FSM[4]
#define Wash5			&FSM[5]
#define Wipe6			&FSM[6]
#define Wash7			&FSM[7]
#define Wipe8			&FSM[8]
#define Wash9			&FSM[9]
#define Wipe10			&FSM[10]
#define Wash11			&FSM[11]
#define Wipe12			&FSM[12]
#define Wash13			&FSM[13]
#define Wipe14			&FSM[14]
#define Wash15			&FSM[15]
#define Wipe16			&FSM[16]
#define Wash17			&FSM[17]
#define Wipe18			&FSM[18]
#define Wash19			&FSM[19]
#define Wipe20			&FSM[20]
#define Wash21			&FSM[21]
#define Wipe22			&FSM[22]
#define Wash23			&FSM[23]
#define Wipe24  		&FSM[24]
#define Wash25			&FSM[25]
#define Wipe26			&FSM[26]
#define Wash27			&FSM[27]
#define Wipe28			&FSM[28]
#define Wash29			&FSM[29]
#define Wipe30			&FSM[30]
#define Wash31    	&FSM[31]
#define Wipe32			&FSM[32]
#define Wash33			&FSM[33]
#define Wipe34			&FSM[34]
#define Wash35			&FSM[35]
#define T20ms					20



void SendDataToLogicAnalyzer(void){
  UART0_DR_R = 0x80|(PA4<<2)|PE50;
}
	struct State{
		uint32_t output;
		uint32_t wait;
		const struct State *Next[4];
	};
	typedef const struct State STyp;
	STyp FSM[36] = {
		{0x01, T20ms, {HOME, Wipe2, Wash3, Wash3}},
		{0x21, T20ms, {Wipe2, Wipe2, Wash3, Wash3}},
		{0x02, T20ms, {Wipe4, Wipe4, Wash5, Wash5}},
		{0x02, T20ms, {Wipe4, Wipe4, Wash5, Wash5}},
		{0x04, T20ms, {Wipe6, Wipe6, Wash7, Wash7}},
		{0x24, T20ms, {Wipe6, Wipe6, Wash7, Wash7}},
		{0x08, T20ms, {Wipe8, Wipe8, Wash9, Wash9}},
		{0x08, T20ms, {Wipe8, Wipe8, Wash9, Wash9}},
		{0x010,T20ms, {Wipe10, Wipe10, Wash11, Wash11}},
		{0x030,T20ms, {Wipe10, Wipe10, Wash11, Wash11}},
		{0x01, T20ms, {Wipe12, Wipe12, Wash13, Wash13}},
		{0x01, T20ms, {Wipe12, Wipe12, Wash13, Wash13}},
		{0x02, T20ms, {Wipe14, Wipe14, Wash15, Wash15}},
		{0x22, T20ms, {Wipe14, Wipe14, Wash15, Wash15}},
		{0x04, T20ms, {Wipe16, Wipe16, Wash17, Wash17}},
		{0x04, T20ms, {Wipe16, Wipe16, Wash17, Wash17}},
		{0x08,T20ms, {Wipe18, Wipe18, Wash19, Wash19}},
		{0x28, T20ms, {Wipe18, Wipe18, Wash19, Wash19}},
		{0x010,T20ms, {Wipe20, Wipe20, Wash21, Wash21}},
		{0x010,T20ms, {Wipe20, Wipe20, Wash21, Wash21}},
		{0x08, T20ms, {Wipe22, Wipe22, Wash23, Wash23}},
		{0x28, T20ms, {Wipe22, Wipe22, Wash23, Wash23}},
		{0x04, T20ms, {Wipe24, Wipe24, Wash25, Wash25}},
		{0x04,T20ms, {Wipe24, Wipe24, Wash25, Wash25}},
		{0x02, T20ms, {Wipe26, Wipe26, Wash27, Wash27}},
		{0x22, T20ms, {Wipe26, Wipe26, Wash27, Wash27}},
		{0x01, T20ms, {Wipe28, Wipe28, Wash29, Wash29}},
		{0x01,T20ms, {Wipe28, Wipe28, Wash29, Wash29}},
		{0x010,T20ms, {Wipe30, Wipe30, Wash31, Wash31}},
		{0x030,T20ms, {Wipe30, Wipe30, Wash31, Wash31}},
		{0x08, T20ms, {Wipe32, Wipe32, Wash33, Wash33}},
		{0x08, T20ms, {Wipe32, Wipe32, Wash33, Wash33}},
		{0x04, T20ms, {Wipe34, Wipe34, Wash35, Wash35}},
		{0x24, T20ms, {Wipe34, Wipe34, Wash35, Wash35}},
		{0x02, T20ms, {HOME, HOME, Wash1, Wash1}},
		{0x02, T20ms, {HOME, HOME, Wash1, Wash1}},
	};
		int volatile t = 1;

int main(void){
		STyp *Pt;
	uint32_t input;	
  TExaS_Init(&SendDataToLogicAnalyzer);    // activate logic analyzer and set system clock to 80 MHz
  SysTick_Init();   
// you initialize your system here
SYSCTL_RCGCGPIO_R |= 0x011;
	t++;
	
 GPIO_PORTE_DEN_R |= 0x03F;
 GPIO_PORTE_DIR_R |= 0x03F;                // PE0-5 are outputs	
 GPIO_PORTA_DEN_R |= 0x0C;
 GPIO_PORTA_DIR_R &= 0xFFFFFFF3;					// PA2-3 are inputs, PA2 for wiper and PA3 for washer
  EnableInterrupts();  
 Pt = &FSM[0];
  while(1){
		
// output
		GPIO_PORTE_DATA_R = Pt->output;
// wait
 				SysTick_Wait10ms(Pt->wait);
				
// input
		input = GPIO_PORTA_DATA_R >> 2;
		
// next	
			Pt = Pt->Next[input];
  }
	
}

