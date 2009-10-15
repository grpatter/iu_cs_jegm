/*
** grpatter
** stack.c
** 
*/

#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

#define MEMSIZE 2048

int TOP;
int mem[MEMSIZE];

void push(int datum){
  if((++TOP) >= MEMSIZE){//if over memory size, do something
    //OVERFLOW();
    printf("Stack Overflow.\n");
	exit(1);//exit with error
  }else{
    mem[TOP] = datum; //point top to new val
  }
  return;
}

bool isEmptyStack(){
  return TOP == -1;//returns true or false
}

int peek(){
  if(isEmptyStack()){
    //UNDERFLOW();
    printf("Stack Underflow.\n");
	exit(1);//exit with error
  }else{
    return mem[TOP];//give back current val, do not move top
  }
}

int pop(){
  int ans = peek();
  
  if(isEmptyStack()){//if empty, we have a problem
    //UNDERFLOW();
    printf("Stack Underflow.\n");
	exit(1);//exit with error
  }else{
	return mem[TOP--];//make sure to decrement
  }
}

void makeEmptyStack(){
  TOP = -1;
  return;
}

void printstack(){
	if(isEmptyStack()){//if at end, start the other side of this recursion
		return;
	}else{
		int t = pop();//preserve value
		printf("%d ",t);//print val
		printstack();//add to recursion list
		push(t);//add val back after we come back
	}

}

void wipe_stack(){
	while(!isEmptyStack()){//while not at end
		pop();//pop the top off
	}
}
