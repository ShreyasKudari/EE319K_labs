// main.cpp
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 4/19/2018 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2017

 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "SlidePot.h"
#include "Images.h"
#include "UART.h"
#include "Timer0.h"
#include "Timer1.h"
#include "sprite_struct.h"
#include "Enemy.h"
#include "Timer2.h"
#include "ground_fire.h"
#include <math.h>
#include <stdlib.h>
#include "Timer3.h"
using namespace std;
SlidePot Player(1500,0);

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);


////////////////////////////////////Sprite definitions/////////////////////////////////////////////////
sprite_t bill={60,9,SmallEnemy20pointB,alive};
sprite_t player={0, 159, PlayerShip0,alive};
//sprite_t ground_fire={0, 0,shortgreen,dead};
Enemy enemy;
ground_fire fire;
int bunkdeath1=2;
int bunk1_flag=0;
int bunk2_flag=0;
int bunkdeath2=2;
int otp=1;
bool win=false;
///////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t time = 0;
volatile uint32_t flag;
void PortF_Init(void){
  //*** students write this ******
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R|=0x20;
	delay++;
	delay++;
	GPIO_PORTF_LOCK_R=GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |=0x11;				//enable commit for PF4 and PF0
	 GPIO_PORTF_AMSEL_R &= 0xEE;	//disable analog functionality on PF4 and PF0
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFF0FFF0)+0x00000000;//configure PF0 and PF4 as GPIO
	GPIO_PORTF_AFSEL_R &= 0xEE;	//disable alt funct on PF0 and PF4
	GPIO_PORTF_DIR_R&=0xEE;			//for song and stop extra credit
	GPIO_PORTF_DEN_R|=0x11;
	GPIO_PORTF_PUR_R=0x11;			//PF4 and PF0 is positive logic

}




void Read_ADC(void){

uint32_t data=ADC_In();
  Player.Save(data);
	
}
int ADCpos2LCDpos(void){
 int ans= ((Player.Distance()-311)*128)/1800;
 if(ans>128)
	 return 128;
 return ans;
}
void Move_player(int x){
	DisableInterrupts();
	
	if(x>player.x)
	{ST7735_DrawBitmap(player.x, 159, clear, x-player.x, 8);
	}
	else if(x<player.x)
	{ST7735_DrawBitmap(x+18, 159, clear, player.x-x, 8);
	}
	

ST7735_DrawBitmap(x, 159, PlayerShip0, 18,8); // player ship middle bottom
		player.x=x;
	EnableInterrupts();
}
int last_per_col(int col){
	for(int i=2;i>=0;i--)
	{ 
		if(enemy.enemyblock[i][col].life==alive)
			return i;
	
	}

}
//////////////////returns x value of closest enemy//////////////////////
Enemy::sprite_t closest_enemy(){
int closest_xval=128;
int val;
int closest_enemyx=-1;	
int closest_enemyy=-1;
	int last_each_col;
for(int i=0;i<4;i++)
	{ last_each_col=last_per_col(i);
		if(enemy.enemyblock[last_each_col][i].life==alive)
		{ 
			val=enemy.enemyblock[last_each_col][i].x-player.x;
			if(val<0)
				val=0-val;
			if(val<closest_xval)
			{closest_xval=val;
				closest_enemyx=i;
				closest_enemyy=last_each_col;
			}
		}
	
	
	}
 return enemy.enemyblock[closest_enemyy][closest_enemyx];
}
////////////////Incoming !//////////////////////
/////////////////// Launches Enemy missiles at random times////////////////////////
///////////////// 3 missiles at a time on the screen ///////////////////////
void ripbunker(){
for(int k=0;k<10;k++)
	{if(fire.bunker1.x<=enemy.enemy_missiles[0].x+k&&fire.bunker1.x+17>enemy.enemy_missiles[0].x+k)
		{ST7735_DrawBitmap(fire.bunker1.x,fire.bunker1.y, clear, 17,13);
			for(int i=0;i<13;i++)
				{for(int j=0;j<17;j++)
					{fire.bunkerdmg1[i][j]=0;
					}
				}
				bunk1_flag=1;
				return;
		}
		else if(fire.bunker2.x<=enemy.enemy_missiles[0].x+k&&fire.bunker2.x+17>enemy.enemy_missiles[0].x+k)
		{
			ST7735_DrawBitmap(fire.bunker2.x,fire.bunker2.y, clear, 17,13);
			for(int i=0;i<13;i++)
				{for(int j=0;j<17;j++)
					{fire.bunkerdmg1[i][j]=0;
					}
				}
				bunk2_flag=1;
				return;
		}
		
	}

} 
void incoming(void){
	if(enemy.enemy_missiles[0].life==dead)
	{ if(enemy.remaining>0)
		{enemy.enemy_missiles[0].life=enemy.alive;
		enemy.enemy_missiles[0].x=closest_enemy().x+6;
		enemy.enemy_missiles[0].y=closest_enemy().y+18;
		
		}
	}
	else 
	return;
}
 //75,130   32,130
//size = 17x13 bunker dimensions
bool checkarray1(){
for(int i=0;i<13;i++)
	{for(int j=0;j<17;j++)
		{
		  if(fire.bunkerdmg1[i][j]==1)
					return true;
		}
	
	}
	return false;
}
bool checkarray2(){
for(int i=0;i<13;i++)
	{
	for(int j=0;j<17;j++)
		{ if(fire.bunkerdmg2[i][j]==1)
			  return true;
		}
	}
	return false;
}
void col_enemy_bunker(void){
	for(int i=0;i<4;i++)
	{ if(enemy.enemyblock[enemy.downmost][i].life==alive)
		{  
				if(otp==1&&enemy.enemyblock[enemy.downmost][i].y>=112)
				{ if(checkarray1())
						{//ST7735_DrawBitmap(fire.bunker1.x,fire.bunker1.y, clear, 17,13);
						bunk1_flag=1;
						}
					if(checkarray2())
					{//ST7735_DrawBitmap(fire.bunker2.x,fire.bunker2.y, clear, 17,13);
						bunk2_flag=1;
					}
					for(int i=0;i<13;i++)
					{
					for(int j=0;j<17;j++)
					{
						fire.bunkerdmg1[i][j]=0;
						fire.bunkerdmg2[i][j]=0;
					}
					}
				return;
				}
		
		}
	
	
	}


}
void col_enemy_land(void){
for(int i=0;i<4;i++)
	{
		if(enemy.enemyblock[enemy.downmost][i].y>128)
		{player.life=dead;
		return;}
	}

}
void motion(void){
col_enemy_land();
enemy.motion();
col_enemy_bunker();
if(enemy.attack_count==0)
{enemy.attack_count=10;
 incoming();
}
else 
	enemy.attack_count--;
if(bunk1_flag==1)
{ ST7735_DrawBitmap(fire.bunker1.x,fire.bunker1.y,explosion2, 17,13);
	if(bunkdeath1==0)
	{bunk1_flag=0;
		otp=0;
		bunkdeath1=3;
		ST7735_DrawBitmap(fire.bunker1.x,fire.bunker1.y,clear, 17,13);
	}
	bunkdeath1--;
	
}
if(bunk2_flag==1)
{ST7735_DrawBitmap(fire.bunker2.x,fire.bunker2.y, explosion2, 17,13);
	if(bunkdeath2==0)
	{ bunk2_flag=0;
		bunkdeath2=3;
		otp=0;
		ST7735_DrawBitmap(fire.bunker2.x,fire.bunker2.y,clear, 17,13);
	}
	bunkdeath2--;
	
}
enemy.displayenemies();

}
bool gnd_missile_col(){
	int toprow_missile=fire.ground_fire.y-14;
	for(int i=0;i<3;i++){
for(int j=0;j<4;j++)
	{
		if(enemy.enemyblock[i][j].life==alive){
			if(toprow_missile<=enemy.enemyblock[i][j].y){
				for(int k=0;k<6;k++)
				{	
					if(enemy.enemyblock[i][j].x<fire.ground_fire.x+k&&enemy.enemyblock[i][j].x+16>fire.ground_fire.x+k)
					{
						fire.ground_fire.life=fire.dead;
						enemy.enemyblock[i][j].life=enemy.dead;
						ST7735_DrawBitmap(fire.ground_fire.x, fire.ground_fire.y,clear , 6, 14);
						ST7735_DrawBitmap(enemy.enemyblock[i][j].x, enemy.enemyblock[i][j].y,clear , 16, 10);
						enemy.remaining-=1;
						enemy.score+=10;
						if(enemy.remaining==0)
						{ int temp=enemy.score;
								if(enemy.enemy_missiles[0].life==alive)
								{ enemy.enemy_missiles[0].life=enemy.dead;
								ST7735_DrawBitmap(enemy.enemy_missiles[0].x, enemy.enemy_missiles[0].y, clear, 10, 16);}
							enemy.initialize_enemies();
						 enemy.score=temp;
								otp=1;
							fire.defense();
						ST7735_DrawBitmap(fire.bunker1.x,fire.bunker1.y, bunker, 17,13);
						ST7735_DrawBitmap(fire.bunker2.x,fire.bunker2.y, bunker, 17,13);
					
							//what happens when you win
							}
						return true;
					}
				}
			}
		}
	}
}
	return false;
}

void bunker1ff(int k){
	int exist_flag=0;
	for(int i=0;i<6;i++)
	{
		if(fire.bunker1.x<=fire.ground_fire.x+i && fire.bunker1.x+17>fire.ground_fire.x+i)
		{
			if(fire.bunkerdmg1[fire.ground_fire.y-13-118][fire.ground_fire.x+i-32]==1)
			{exist_flag=1;
			break;}
		}
	
	
	}
	if(exist_flag==0)
		return;

 ST7735_DrawBitmap(fire.ground_fire.x, fire.ground_fire.y,clear , 6, 14);
	fire.ground_fire.life=fire.dead;
	
	for(int i=0;i<6;i++)
	{
		if(fire.bunker1.x<=fire.ground_fire.x+i && fire.bunker1.x+17>fire.ground_fire.x+i)
		{
			fire.bunkerdmg1[fire.ground_fire.y-13-118][fire.ground_fire.x+i-32]=0;
		}
	
	}
	

}

void bunker2ff(int k){
	int exist_flag=0;
	for(int i=0;i<6;i++)
	{
		if(fire.bunker2.x<=fire.ground_fire.x+i && fire.bunker2.x+17>fire.ground_fire.x+i)
		{
			if(fire.bunkerdmg2[fire.ground_fire.y-13-118][fire.ground_fire.x+i-75]==1)
			{exist_flag=1;
			break;}
		}
	 
	}
	if(exist_flag==0)
		return;

 ST7735_DrawBitmap(fire.ground_fire.x, fire.ground_fire.y,clear , 6, 14);
	fire.ground_fire.life=fire.dead;

	for(int i=0;i<6;i++)
	{
		if(fire.bunker2.x<=fire.ground_fire.x+i && fire.bunker2.x+17>fire.ground_fire.x+i)
		{
			fire.bunkerdmg2[fire.ground_fire.y-13-118][fire.ground_fire.x+i-75]=0;
		}
	
	}


}

void bunkercol(){
	int missiletip=fire.ground_fire.y-13;
if(missiletip<=fire.bunker1.y&&missiletip>=fire.bunker1.y-12)
{
	
	for(int k=0;k<6;k++)
	{ if(fire.bunker1.x<=fire.ground_fire.x+k && fire.bunker1.x+17>fire.ground_fire.x+k)
		{ bunker1ff(k);
			return;
		}
		else if(fire.bunker2.x<=fire.ground_fire.x+k && fire.bunker2.x+17>fire.ground_fire.x+k)
		{bunker2ff(k);
			return;
		}
		
	}


}

}



void bunker1_ef(){
	int exist_flag=0;
		for(int i=0;i<10;i++)
	{
		if(fire.bunker1.x<=enemy.enemy_missiles[0].x+i && fire.bunker1.x+17>enemy.enemy_missiles[0].x+i)
		{
			if(fire.bunkerdmg1[enemy.enemy_missiles[0].y-118][enemy.enemy_missiles[0].x+i-32]==1)
			{exist_flag=1;
			break;}
		}
	
	
	}
	if(exist_flag==0)
		return;
	ST7735_DrawBitmap(enemy.enemy_missiles[0].x, enemy.enemy_missiles[0].y,clear , 10, 16);
	enemy.enemy_missiles[0].life=enemy.dead;
	
	for(int i=0;i<10;i++)
	{
		if(fire.bunker1.x<=enemy.enemy_missiles[0].x+i && fire.bunker1.x+17>enemy.enemy_missiles[0].x+i)
		{
			fire.bunkerdmg1[enemy.enemy_missiles[0].y-118][enemy.enemy_missiles[0].x+i-32]=0;
		}
	
	}
	
}
void bunker2_ef(){
int exist_flag=0;
		for(int i=0;i<10;i++)
	{
		if(fire.bunker2.x<=enemy.enemy_missiles[0].x+i && fire.bunker2.x+17>enemy.enemy_missiles[0].x+i)
		{
			if(fire.bunkerdmg2[enemy.enemy_missiles[0].y-118][enemy.enemy_missiles[0].x+i-75]==1)
			{exist_flag=1;
			break;}
		}
	
	
	}
	if(exist_flag==0)
		return;
	ST7735_DrawBitmap(enemy.enemy_missiles[0].x, enemy.enemy_missiles[0].y, clear , 10, 16);
	enemy.enemy_missiles[0].life=enemy.dead;
	
	for(int i=0;i<10;i++)
	{
		if(fire.bunker2.x<=enemy.enemy_missiles[0].x+i && fire.bunker2.x+17>enemy.enemy_missiles[0].x+i)
		{
			fire.bunkerdmg2[enemy.enemy_missiles[0].y-118][enemy.enemy_missiles[0].x+i-75]=0;
		}
	
	}
	

}
//bunker size is 17 wide 13 high
//enemy missile size is 10  wide 16 high


void bunkerclear_col(void){
	int left=-1;
	int right=-1;
for(int k=0;k<10;k++)
		{ 
			if(fire.bunker1.x<=enemy.enemy_missiles[0].x+k&&fire.bunker1.x+17>enemy.enemy_missiles[0].x+k)
			{ 
				if(left==-1)
				{ left=enemy.enemy_missiles[0].x+k;
				}
				else if(k==9)
				{
				  right=enemy.enemy_missiles[0].x+9;
				}
				else if(enemy.enemy_missiles[0].x+k==fire.bunker1.x+16)
				{
					right=fire.bunker1.x+16;
				}
				int level=118;
				for(int i=0;i<12;i++)
				{
					for(int j=left;j<=right;j++)
					{
					  fire.bunkerdmg1[level+i][j]=0;
					}
				}
			
				return;
			}
			else if(fire.bunker2.x<=enemy.enemy_missiles[0].x+k&&fire.bunker2.x+17>enemy.enemy_missiles[0].x+k){
				//bunker2_cc();
				if(left==-1)
				{ left=enemy.enemy_missiles[0].x+k;
				}
				else if(k==9)
				{
				  right=enemy.enemy_missiles[0].x+9;
				}
				else if(enemy.enemy_missiles[0].x+k==fire.bunker2.x+16)
				{
					right=fire.bunker2.x+16;
				}
				int level=118;
				for(int i=0;i<12;i++)
				{
					for(int j=left;j<=right;j++)
					{
					  fire.bunkerdmg2[level+i][j]=0;
					}
				}
			
				return;
			}
			
		}
}

void bunkercol_enemy(){
 int emistip=enemy.enemy_missiles[0].y;
	if(emistip>=fire.bunker1.y-12 && emistip<=fire.bunker1.y)
	{ 
		for(int k=0;k<10;k++)
		{
			if(fire.bunker1.x<=enemy.enemy_missiles[0].x+k&&fire.bunker1.x+17>enemy.enemy_missiles[0].x+k)
			{ bunker1_ef();
				return;
			}
			else if(fire.bunker2.x<=enemy.enemy_missiles[0].x+k&&fire.bunker2.x+17>enemy.enemy_missiles[0].x+k){
				bunker2_ef();
				return;
			}
			
		}
		
		
	}



}
void col_enemy_player(void)
{	//enemy block bunkers level collision
	// if missile is in bunker x bounds but below bunker, then destroy the entire column
	//enemy reaches last row detection
	//enemy player collision
	//pause
	//descriptions of why you lost
	
}	


//player is 18x8
//grounded on 159
//enemy fire is 10x16
void col_enemymissile_player(void){
if(enemy.enemy_missiles[0].y>=152)
{
	for(int i=0;i<10;i++)
	{ if(enemy.enemy_missiles[0].x+i>=player.x && enemy.enemy_missiles[0].x+i<player.x+17)
		{player.life=dead;
		 return;
		}
	
	}

}
}

void missile_propagate(){

	if(fire.ground_fire.life==dead)
{return;}
	fire.missile_propagate();
  bunkercol();
	if(gnd_missile_col())
	{ enemy.leftmost=enemy.leftmostupdate();
		enemy.rightmost=enemy.rightmostupdate();
	  enemy.lowestupdate();
	}
}
void enemy_missile_motion(){
if(enemy.enemy_missiles[0].life==dead)
	return;

enemy.moveattack();
bunkercol_enemy();
col_enemymissile_player();
}


void clock(void){
  time++;
}


int main(void){
	
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
  Random_Init(1);
  Output_Init();
	ADC_Init();				//initializing ADC for slide pot player control
	PortF_Init();			//initialize PortF buttons for shoot and pause
										//PF0 - start / stop
										//PF4 - shoot
  Timer0_Init(&Read_ADC,1333333); // 60 Hz THIS TIMER IS USED FOR ADC higher priority than timer1
  Timer1_Init(&missile_propagate,333333); // 60Hz
	Timer2_Init(&motion,18000000);//for enemy block movement 
	Timer3_Init(&enemy_missile_motion,800000);
	enemy.initialize_enemies();
	fire.defense();
	enemy.displayenemies();
	
  EnableInterrupts();
	//ST7735_DrawBitmap(0,160, spaceinvaders_logo, 128,160); // player ship middle bottom
	ST7735_DrawBitmap(fire.bunker1.x,fire.bunker1.y, bunker, 17,13);
	ST7735_DrawBitmap(fire.bunker2.x,fire.bunker2.y, bunker, 17,13);
	
	
  while(player.life==alive)
	{ 
		
		if(((GPIO_PORTF_DATA_R&0x10)==0)&&fire.ground_fire.life==dead)
		{ 
			fire.ground_fire.x=player.x+7;
			fire.ground_fire.y=150;
			fire.ground_fire.life=fire.alive;
		}
		
		Move_player(ADCpos2LCDpos());
	}
	
	
	
	
/*  ST7735_DrawBitmap(64, 159, PlayerShip0, 18,8); // player ship middle bottom
  ST7735_DrawBitmap(64, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  ST7735_DrawBitmap(100, 9, SmallEnemy30pointB, 16,10);
  while(bill.life == alive){
    while(flag==0){};
    flag = 0;
    ST7735_DrawBitmap(bill.x,bill.y,bill.image,16,10);
  }*/
	DisableInterrupts();

  ST7735_FillScreen(0x0000);            // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString((char*)"GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_SetTextColor(ST7735_WHITE);
  ST7735_OutString((char*)"Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char*)"Score: ");
  ST7735_SetCursor(7, 3);
  ST7735_SetTextColor(ST7735_WHITE);
	 ST7735_OutUDec(enemy.score);
  while(1){
   
  }

}






