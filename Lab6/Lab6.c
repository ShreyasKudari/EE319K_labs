// Lab6.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 3-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/5/18 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include "DAC.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
//const uint32_t dac_test[16]={0,1,7,15,16,17,18,31,32,33,47,48,49,62,63};
extern uint32_t attack;							
extern uint32_t decay;
extern uint8_t en_flag;

int main(void){ 
uint8_t music_flag=0;
int hb_toggle=0;
uint32_t option=0;	
 TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  Piano_Init();
  Sound_Init();
	//Heart beat led init
	GPIO_PORTF_DEN_R|=0x02;
	GPIO_PORTF_DIR_R|=0x02;
	
	Timer0A_Init(Song_Play,0xFFFFFFFF);								//initialize TIMER0A for song(extra credit #1)
	int song_input=0;																	// for on-board button which plays the song
  // other initialization
  EnableInterrupts();																//I bit for globally enabling interrupts
  while(1){ 
		GPIO_PORTF_DATA_R^=(hb_toggle/100000)<<1;					//heartbeat toggles after 100,000 iterations
		hb_toggle=(hb_toggle+1)%100001;
		option=Piano_In();																//get input from breadboard buttons to play single notes
		song_input=GPIO_PORTF_DATA_R&0x11;			//isolate bit 4

		
		if(song_input==1 && music_flag==0)																//PF4 is negative logic 
		{Song(1);											//if PF4 is pressed play song and skip rest of the loop
			music_flag=1;
			//continue;
		}
		else if(song_input==16 && music_flag==1)
		{stop();	
			music_flag=0;																				//stop song play
		}
		if(music_flag==0){
																	//32 bit wave struct															
		if(option==1)					//R=fbus/(fout*N)
		{Sound_Play(3792);		//E5---> 80M/(659.25*32)~ 3792									
		}
		else if(option==2)
		{Sound_Play(3189);			//G5----> 80M/(783.99*32) ~3189								
		}
		else if(option==4)
		{Sound_Play(5682);			//A4 ----> 80M/(440*32) ~5682									
		}
		else
		{Sound_Play(0);
			attack=0;							//reinitialize envelope variables attack and decay
			decay=0;
			en_flag=0;						//make this flag 0 to indicate to start next time with attack
														// en_flag=0	-----> attack
														// en_flag=1 	-----> decay
		}
	}
	
		
  }    
/*	     uint32_t Data;
   // 0 to 15 DAC output
		// Uncomment this code to test the DAC for deliverables
  DAC_Init();
  for(int i=0;;i++) {
		if(i==16)
			i=0;
		Data=dac_test[i];
		
    DAC_Out(Data);
		
    
  }*/


}



