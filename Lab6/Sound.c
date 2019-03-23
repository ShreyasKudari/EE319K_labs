// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: Shreyas Kudari
// Date Created: 3/6/17 
// Last Modified: 3/5/18 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "../inc/tm4c123gh6pm.h"
#define 	A  5682
#define 	B	 5062
#define 	Cs 4510
#define 	D  4257
#define 	E  3792
#define 	Fs 3378
#define 	G  3189
#define 	Ah 2841
#define 	Fsl 6757
#define 	Gl 6378
#define 	El 7584
#define 	Dl 8513
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)();   // user function


const uint8_t wave[64]={32,35,38,41,44,47,49,52,54,56,58,59,61,62,62,63,63,63,62,62,61,
59,58,56,54,52,49,47,44,41,38,35,32,29,26,23,20,17,15,12,10,8,6,5,3,2,2,1,1,1,2,2,3,5,6,
8,10,12,15,17,20,23,26,29};
const uint8_t wave1[32]={32,33,35,36,37,38,38,39,39,39,38,38,37,36,35,33,32,31,29,28,27,26,
26,25,25,25,26,26,27,28,29,31};			

const uint8_t guitar[64]={0,20,20,19,16,12,8,4,3,5,10,17,
	26,33,38,41,42,40,36,29,21,13,9,
	9,14,23,34,45,52,54,51,45,38,31,
	26,23,21,20,20,20,22,25,27,29,
30,29,27,22,18,13,11,10,11,13,13,13,13,13,14,16,18,20,20,20};


//Canon in D song notes
uint32_t song1[112]={Ah,Fs,G,Ah,Fs,G,Ah,A,B,Cs,D,E,Fs,G,	
Fs,D,E,Fs,Fsl,Gl,A,B,A,Gl,A,Fsl,Gl,A,
Gl,B,A,Gl,Fsl,El,Fsl,El,Dl,El,Fsl,Gl,A,B,
Gl,B,A,B,Cs,D,A,B,Cs,D,E,Fs,G,Ah,
Fs,D,E,Fs,E,D,E,A,B,E,Fs,E,D,Cs,
D,B,Cs,D,Dl,El,Fsl,Gl,Fsl,El,Fsl,D,Cs,D,
B,D,Cs,B,A,Gl,A,Gl,Fsl,Gl,A,B,Cs,D,
B,D,Cs,D,Cs,B,Cs,D,E,D,Cs,D,B,Cs};

uint8_t count=0;
uint8_t timecount=0;
uint8_t timelen=0;
uint8_t flag=0;
uint8_t songlen=0;
uint32_t attack=0;
uint32_t decay=0;
uint8_t mod=0;
uint8_t en_flag=0;
uint32_t* songchoice;

const uint32_t temp=0x434A900;	//1 beat time delay value for 68BPM

//note durations based on 68BPM tempo, 4/4
uint32_t time_note1[14]={temp/2,temp/4,temp/4,temp/2,temp/4,temp/4,		//consists of 8th and 16th notes 
temp/4,temp/4,temp/4,temp/4,temp/4,temp/4,temp/4,temp/4};
// **************Sound_Init*********************
// Initialize digital outputs and SysTick timer
// Called once, with sound/interrupts initially off
// Input: none
// Output: none
void Sound_Init(void){							//initializes Systick Timer, the reload value of which controls freq
	DAC_Init();
	NVIC_ST_CTRL_R=0;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_RELOAD_R=800000-1;
	NVIC_SYS_PRI3_R=(NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
	NVIC_ST_CTRL_R=0x07;
	}
void Timer0A_Init(void(*task)(), uint32_t period){long sr;		//to change notes for the song
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  PeriodicTask = task;          // user function
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = period-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
  EndCritical(sr);
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  (*PeriodicTask)();                // execute user task
}



void stop(void){
	if(flag==1)							//if song's been playing disable the TIMER0A 
	{TIMER0_TAILR_R=0;
	flag=0;

	}
}

// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Sound continues until Sound_Play called again
// This function returns right away and sound is produced using a periodic interrupt
// Input: interrupt period
//           Units of period to be determined by YOU
//           Maximum period to be determined by YOU
//           Minimum period to be determined by YOU
//         if period equals zero, disable sound output
// Output: none
void Sound_Play(uint32_t period){

	if(period==0)
	{NVIC_ST_RELOAD_R=0;
	}
	else {
		NVIC_ST_RELOAD_R=period-1;						//sets reload value and thus controls frequency
	}																				//RELOAD=Fbus/(Fout*32);

	
	
	}

void Song(uint8_t choice){
	 if(choice==1&&flag==0)								//song select
	 {songchoice=song1;
		 TIMER0_TAILR_R=0x00FFFFFF;
		 count=0;
		 timecount=0;
		 songlen=112;
		 timelen=14;
		 flag=1;
	 }
	 
	}
	uint8_t envelope(int8_t val)									//envelope function modulates the amplitude with attack and decay
	{ if(en_flag==0)
		{mod=attack/32;
			attack++;
			if((val-32)>0)
		{ val+=mod;
		}	//add
		else if((val-32)<0)
			{val-=mod;
			}	//subtract
			
			if(mod==24)
			{	
			en_flag=1;
			attack=0;
			mod=0;}
		}
		else if(en_flag==1)
		{ 
		 mod=decay/500;
		 decay++;
			
			if((val-32)>0)
		{ 
		 val=val+24-mod;
			if(val-32<0)
			val=32;
				}	//subtract
		else if((val-32)<0)
			{val=val-24+mod;
				if(val-32>0)
				val=32;
				
			}	//add
			
		}
	
		return val;
	
}
	

	void Song_Play(void){
		TIMER0_TAILR_R=time_note1[timecount];
		timecount=(timecount+1)%timelen;
		Sound_Play(songchoice[count]);
		count=(count+1)%songlen;  
	  if(flag==1)													//If song play is enabled, then reinitialize envelope vars before 
		{attack=0;													//starting next note.
			decay=0;}
		
	}
	
	
void SysTick_Handler(void){								//outputs one wave datapoint to DAC
	static int i=0;
	DAC_Out(envelope(wave1[i]));
	i=(i+1)%32;
	
}