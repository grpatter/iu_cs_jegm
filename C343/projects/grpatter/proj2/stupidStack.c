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

int mem[MEMSIZE];

void push(int datum){
  enQueue(datum);//add it
}

bool isEmptyStack(){
  return isEmpty();//return bool
}

int peek(){
  //Nothing to see here, move along.
}

int pop(){
  return fromQueue();//return top from queue
}

void makeEmptyStack(){
  return queueInit();//init and return
}
