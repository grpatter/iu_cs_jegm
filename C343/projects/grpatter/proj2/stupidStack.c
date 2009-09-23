/*
** queueStack.c
** Greg Patterson (grpatter at indiana dot edu)
** 9/22/2009
*/

#include <stdio.h>
#include <stdbool.h>
#include "stack.h"
#include "queue.h"

#define MEMSIZE 1024

Queue q;

int mem[MEMSIZE];

void push(int datum){
  enQueue(datum);//add it
}

bool isEmptyStack(){
  return isEmpty();//return bool
}

int peek(){
	int d;
	if(isEmptyStack()){
		printf("Nothing to see here, move along...\n");
		return 0;
	}else{
		d = fromQueue();
		return d;
	}
}

int pop(){
	int d;
	if(isEmptyStack()){
		printf("Nothing to see here, move along...\n");
		return 0;
	}else{
		d = fromQueue();
		return d;
	}
}

void makeEmptyStack(){
  return queueInit();//init and return
}
