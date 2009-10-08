// Project 2, C343: Data Structures, Indiana University
// File: seqStack.c
// Name: Michael Adams
// Username: adamsmd
// Date: 2009 Sept.

#include "stack.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int values[MEMORY_SIZE];
static int top; // inclusive top element of stack
static bool initialized = false;

void initStack() {
  assert(!initialized);
  top = -1;
  initialized = true;
  return;
}

bool isEmpty() {
  assert(initialized);
  return length() == 0;
}

int length() {
  assert(initialized);
  return top + 1;
}

void push(int value) {
  assert(initialized);
  if (top == MEMORY_SIZE) {
    printf("OVERFLOW\n");
    exit(-1);
  } else {
    values[++top] = value;
    return;
  }
}

int pop() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    return values[top--];
  }
}

int peek() {
  assert(initialized);
  if (isEmpty()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    return values[top];
  }
}
