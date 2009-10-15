// Project 2, C343: Data Structures, Indiana University
// File: linkedStack.c
// Name: Michael Adams
// Username: adamsmd
// Date: 2009 Sept.

#include "stack.h"
#include "AVAIL.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static Pointer top;
static int stackLength;
static bool initialized = false;

void initStack() {
  assert(!initialized);
  memoryInit();
  top = NIL;
  stackLength = 0;
  initialized = true;
  return;
}
  
bool isEmpty() {
  assert(initialized);
  return length() == 0;
}

int length() {
  assert(initialized);
  return stackLength;
}

void push_int(int value) {
  assert(initialized);
  Pointer p = fromAVAIL();
  memory[p].data.myunion.num = value;
  memory[p].data.tag = 0;
  memory[p].data.next = top;
  top = p;
  stackLength++;
  return;
}
void push_char(char value) {
  assert(initialized);
  Pointer p = fromAVAIL();
  if(strcmp(input, "X") == 0 || strcmp(input, "Y") == 0 || strcmp(input, "Z") == 0){
	memory[p].data.myunion.var = value;
	memory[p].data.tag = 1;
  }else{
	memory[p].data.myunion.op = value;
	memory[p].data.tag = 2;
  }
  memory[p].data.next = top;
  top = p;
  stackLength++;
  return;
}

int pop_int() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    stackLength--;
    Pointer p = top;
    top = memory[p].data.next;
    int value = memory[p].data.myunion.num;
    toAVAIL(p);
    return value;
  }
}
char pop_char() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    stackLength--;
    Pointer p = top;
    top = memory[p].data.next;
	if(peek_tag() == 1){
		char value = memory[p].data.myunion.var;
	}else{
		char value = memory[p].data.myunion.op;
    toAVAIL(p);
    return value;
  }
}

int peek_tag() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    return memory[top].data.tag;
  }
}
int peek_int() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    return memory[top].data.myunion.num;
  }
}
int peek_char() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
	if(peek_tag() == 1){
		return memory[top].data.myunion.var;
	}else{
		return memory[top].data.myunion.op;
	}
  }
}
