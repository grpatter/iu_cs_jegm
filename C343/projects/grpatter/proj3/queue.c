// Project 2, C343: Data Structures, Indiana University
// File: queue.c
// Name: Michael Adams
// Username: adamsmd
// Date: 2009 Sept.

#include "queue.h"
#include "AVAIL.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static Pointer front, rear;
static int queueLength;
static bool initialized = false;

void initQueue() {
  assert(!initialized);
  memoryInit();
  front = NIL;
  // rear undefined
  queueLength = 0;
  initialized = true;
  return;
}

bool isEmptyQueue() {
  assert(initialized);
  return lengthQueue() == 0;
}

int lengthQueue() {
  assert(initialized);
  return queueLength;
}

void enQueue(int value) {
  assert(initialized);
  Pointer p = fromAVAIL();
  memory[p].data.value = value;
  memory[p].data.next = NIL;

  if (isEmptyQueue()) {
    front = p;
  } else {
    memory[rear].data.next = p;
  }

  rear = p;
  queueLength++;
  return;
}

int fromQueue() {
  assert(initialized);
  if (isEmptyQueue()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    queueLength--;
    Pointer p = front;
    front = memory[p].data.next;
    int value = memory[p].data.value;
    toAVAIL(p);
    return value;
  }
}

int peekQueue() {
  assert(initialized);
  if (isEmptyQueue()) {
    printf("UNDERFLOW\n");
    exit(-1);
  } else {
    return memory[front].data.value;
  }
}
