/* 
** linkedStack.c
** Greg Patterson (grpatter at indiana dot edu)
** 9/22/2009
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "AVAIL.h"
#include "stack.h"

Pointer cur_pt;//hold loc

void push(int datum){
  cur_pt = fromAVAIL();//get some space
  memory[cur_pt].data.queueData = datum;//add it to the list
}

int pop(){
	if(!isEmptyStack()){
		int data =  memory[cur_pt].data.queueData;//grab the data
		toAVAIL(cur_pt);//clear the mem space
		cur_pt--;//decrease index (hopefully above + this happen very fast ;))
		return data;//return the data grabbed
	}else{
		return 0;//nothing
	}
}

void makeEmptyStack(){
  memoryInit();//initialize for use
  cur_pt = -1;
}

bool isEmptyStack(){
	if(cur_pt == -1){//check current loc
		return true;
	}else{
		return false;
	}
}

int peek(){
	if(!isEmptyStack()){
		return memory[cur_pt].data.queueData;//return data here, but dont change any access params
	}else{
		return 0;//nothing
	}
}

