/*
** stack.c
** Greg Patterson (grpatter at indiana dot edu)
** 9/22/2009
*/


#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

#define MEMSIZE 2048

int TOP;
int mem[MEMSIZE];

void overflow(){//show overflow message
    printf("OVERFLOW");
}
void underflow(){//show underflow message
    printf("UNDERFLOW");
}

void push(int datum){
	if((++TOP) >= MEMSIZE){//if adding would go over alottment...
		overflow();//show overflow
	}else{
		mem[TOP] = datum;//otherwise add it (increment already occurred)
	}
	return;
}

bool isEmptyStack(){
  return TOP == -1;//returns t/f
}

int peek(){
	if(isEmptyStack()){//if already empty
		underflow();//show underflow
	}else{
		return mem[TOP];//otherwise return val
	}
}

int pop(){  
	if(isEmptyStack()){//if already empty
		underflow();//show underflow
	}else{
		return mem[TOP--];//otherwise return val, decrement top location
	}
}

void makeEmptyStack(){//give an empty stack to work with
  TOP = -1;
  return; //Space is nil
}
