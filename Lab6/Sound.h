// Sound.h
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/20/18 
// Lab number: 6
// Hardware connections

#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>
// Header files contain the prototypes for public functions
// this file explains what the module does

// **************Sound_Init*********************
// Initialize digital outputs and SysTick timer
// Called once, with sound/interrupts initially off
// Input: none
// Output: none
void Sound_Init(void);
void Song(uint8_t);
void Song_Play(void);
void Timer0A_Init(void(*task)(),uint32_t);
void stop(void);
uint8_t envelope(int8_t);
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
void Sound_Play(uint32_t period);

#endif

