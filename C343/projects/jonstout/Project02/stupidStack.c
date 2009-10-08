/*
** FILE: queueStack.c
** NAME: Jonathan M. Stout | jonstout@indiana.edu
** DATE: 9/21/2009
*/

#include <stdio.h>
#include <stdbool.h>
#include "stack.h"
#include "queue.h"

#define MEMSIZE 2048

int mem[MEMSIZE];

void push(int datum)
{
  enQueue(datum);
}

bool isEmptyStack()
{
  return isEmpty();
}

int peek()
{
  //NULL
}

int pop()
{
  return fromQueue();
}

void makeEmptyStack()
{
  return queueInit();
}
