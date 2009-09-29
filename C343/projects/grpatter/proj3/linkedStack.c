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

void pushzzz(int value) {
  assert(initialized);
  Pointer p = fromAVAIL();
  memory[p].data.value = value;
  memory[p].data.next = top;
  top = p;
  stackLength++;
  return;
}

int pop() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    stackLength--;
    Pointer p = top;
    top = memory[p].data.next;
    int value = memory[p].data.value;
    toAVAIL(p);
    return value;
  }
}

int peek() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    return memory[top].data.value;
  }
}
