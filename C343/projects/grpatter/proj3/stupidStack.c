// Project 2, C343: Data Structures, Indiana University
// File: stupidStack.c
// Name: Michael Adams
// Username: adamsmd
// Date: 2009 Sept.

#include "stack.h"
#include "queue.h"
#include <assert.h>
#include <stdbool.h>

bool initialized = false;

void initStack() {
  assert(!initialized);
  initQueue();
  initialized = true;
  return;
}
  
bool isEmpty() {
  assert(initialized);
  return isEmptyQueue();
}

int length() {
  assert(initialized);
  return lengthQueue();
}

void push(int value) {
  assert(initialized);
  enQueue(value);
  return;
}

int pop() {
  assert(initialized);
  return fromQueue();
}

int peek() {
  assert(initialized);
  return peekQueue();
}
