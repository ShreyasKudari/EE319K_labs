// StepperMotorController.c starter file EE319K Lab 5
// Runs on TM4C123
// Finite state machine to operate a stepper motor.  
// Jonathan Valvano
// January 18, 2019

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
#define	WIPE1			&FSM[2]
#define WIPE2			&FSM[4]

void SendDataToLogicAnalyzer(void){
  UART0_DR_R = 0x80|(PA4<<2)|PE50;
}
	struct State{
		uint32_t output[2];
		uint32_t wait;
		const struct State *Next[4];
	};
	typedef const struct State STyp;
	STyp FSM[36] = {
		{{0x01,0}, 10, {&FSM[0], &FSM[2], &FSM[3], &FSM[3]}},
		{{0x01,1}, 10, {&FSM[2], &FSM[2], &FSM[3], &FSM[3]}},
		{{0x02,0}, 10, {&FSM[4], &FSM[4], &FSM[5], &FSM[5]}},
		{{0x02,1}, 10, {&FSM[4], &FSM[4], &FSM[5], &FSM[5]}},
		{{0x04,0}, 10, {&FSM[6], &FSM[6], &FSM[7], &FSM[7]}},
		{{0x04,1}, 10, {&FSM[6], &FSM[6], &FSM[7], &FSM[7]}},
		{{0x08,0}, 10, {&FSM[8], &FSM[8], &FSM[9], &FSM[9]}},
		{{0x08,1}, 10, {&FSM[8], &FSM[8], &FSM[9], &FSM[9]}},
		{{0x010,0}, 10, {&FSM[10], &FSM[10], &FSM[11], &FSM[11]}},
		{{0x010,1}, 10, {&FSM[10], &FSM[10], &FSM[11], &FSM[11]}},
		{{0x01,0}, 10, {&FSM[12], &FSM[12], &FSM[13], &FSM[13]}},
		{{0x01,1}, 10, {&FSM[12], &FSM[12], &FSM[13], &FSM[13]}},
		{{0x02,0}, 10, {&FSM[14], &FSM[14], &FSM[15], &FSM[15]}},
		{{0x02,1}, 10, {&FSM[14], &FSM[14], &FSM[15], &FSM[15]}},
		{{0x04,0}, 10, {&FSM[16], &FSM[16], &FSM[17], &FSM[17]}},
		{{0x04,1}, 10, {&FSM[16], &FSM[16], &FSM[17], &FSM[17]}},
		{{0x08,0}, 10, {&FSM[18], &FSM[18], &FSM[19], &FSM[19]}},
		{{0x08,1}, 10, {&FSM[18], &FSM[18], &FSM[19], &FSM[19]}},
		{{0x010,0}, 10, {&FSM[20], &FSM[20], &FSM[21], &FSM[21]}},
		{{0x010,1}, 10, {&FSM[20], &FSM[20], &FSM[21], &FSM[21]}},
		{{0x08,0}, 10, {&FSM[22], &FSM[22], &FSM[23], &FSM[23]}},
		{{0x08,1}, 10, {&FSM[22], &FSM[22], &FSM[23], &FSM[23]}},
		{{0x04,0}, 10, {&FSM[24], &FSM[24], &FSM[25], &FSM[25]}},
		{{0x04,1}, 10, {&FSM[24], &FSM[24], &FSM[25], &FSM[25]}},
		{{0x02,0}, 10, {&FSM[26], &FSM[26], &FSM[27], &FSM[27]}},
		{{0x02,1}, 10, {&FSM[26], &FSM[26], &FSM[27], &FSM[27]}},
		{{0x01,0}, 10, {&FSM[28], &FSM[28], &FSM[29], &FSM[29]}},
		{{0x01,1}, 10, {&FSM[28], &FSM[28], &FSM[29], &FSM[29]}},
		{{0x010,0}, 10, {&FSM[30], &FSM[20], &FSM[31], &FSM[31]}},
		{{0x010,1}, 10, {&FSM[30], &FSM[20], &FSM[31], &FSM[31]}},
		{{0x08,0}, 10, {&FSM[32], &FSM[32], &FSM[33], &FSM[33]}},
		{{0x08,1}, 10, {&FSM[32], &FSM[32], &FSM[33], &FSM[33]}},
		{{0x04,0}, 10, {&FSM[34], &FSM[34], &FSM[35], &FSM[35]}},
		{{0x04,1}, 10, {&FSM[34], &FSM[34], &FSM[35], &FSM[35]}},
		{{0x02,0}, 10, {&FSM[0], &FSM[0], &FSM[1], &FSM[1]}},
		{{0x02,1}, 10, {&FSM[0], &FSM[0], &FSM[1], &FSM[1]}},
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
		GPIO_PORTE_DATA_R = Pt->output[0];
// wait
 				SysTick_Wait10ms(Pt->wait);
				GPIO_PORTE_DATA_R ^= (Pt->output[1] << 5);			// toggles LED
				//GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R;
				SysTick_Wait10ms(Pt->wait);
// input
		input = GPIO_PORTA_DATA_R >> 2;
		
// next	
			Pt = Pt->Next[input];
  }
	
}

