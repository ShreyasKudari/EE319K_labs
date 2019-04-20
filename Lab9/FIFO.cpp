// FIFO.cpp
// Runs on any microcontroller
// Student names: Shreyas Anand Kudari
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 4/11/2018 

#include <stdint.h>

#include "FIFO.h"
#include "ST7735.h"
#include "print.h"

// A class named Queue that defines a FIFO
Queue::Queue(){
  // Constructor - make FIFO initially empty
  // write this
	PutI=GetI=0;
	
}

// To check whether Queue is empty or not
bool Queue::IsEmpty(void){
  // write this
	if(PutI==GetI)
  return true;

return false;	// replace this line
}

  // To check whether Queue is full or not
bool Queue::IsFull(void){
  // write this
	
	int32_t updated;
	if(PutI==0)
		updated=FIFOSIZE-1;
	else 
		updated=PutI-1;
	if(updated==GetI)
		return true;
	
  return false; // replace this line
}

  // Inserts an element in queue at rear end
bool Queue::Put(char x){
  // write this
	int32_t updated;
	if(PutI==0)
	{updated=FIFOSIZE-1;
	}
	else 
		updated=PutI-1;
	if(updated==GetI)
		return false;
	Buf[PutI]=x;
	PutI=updated;

	
  return true; // replace this line
}

  // Removes an element in Queue from front end. 
bool Queue::Get(char *pt){
  // write this
	if(GetI==PutI)
		return false;
	
	*pt=Buf[GetI];
	if(GetI==0)
		GetI=FIFOSIZE-1;
	else
		GetI--;
  return true; // replace this line
}

  /* 
     Printing the elements in queue from front to rear. 
     This function is only to test the code. 
     This is not a standard function for Queue implementation. 
  */
void Queue::Print(void){
  // write this


}


