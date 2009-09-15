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
  if((++TOP) >= MEMSIZE){
    //OVERFLOW();
    printf("Stack Overflow.\n");
	exit(1);
  }else{
    mem[TOP] = datum;
  }
  return;
}

bool isEmptyStack(){
  return TOP == -1;
}

int peek(){
  if(isEmptyStack()){
    //UNDERFLOW();
    printf("Stack Underflow.\n");
	exit(1);
  }else{
    return mem[TOP];
  }
}

int pop(){
  int ans = peek();
  
  if(isEmptyStack()){
    //UNDERFLOW();
    printf("Stack Underflow.\n");
	exit(1);
  }else{
	return mem[TOP--];
  }
}

void makeEmptyStack(){
  TOP = -1;
  return;
}

void printstack(){
	if(isEmptyStack()){
		return;
	}else{
		int t = pop();
		printf("%d\n",t);
		printstack();
		push(t);
	}

}
