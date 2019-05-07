
#ifndef __TIMER2INTS_H__ // do not include more than once
#define __TIMER2INTS_H__

#include <stdint.h>


 
void Timer2_Init(void(*task)(void), unsigned long period);

#endif // __TIMER2INTS_H__
