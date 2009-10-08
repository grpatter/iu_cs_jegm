#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "AVAIL.h"
#include "stack.h"

Pointer currentPosition;

void push(int datum)
{
  currentPosition = fromAVAIL();
  memory[currentPosition].data.queueData = datum;
}

int pop()
{
  int data =  memory[currentPosition].data.queueData;
  toAVAIL(currentPosition);
  currentPosition--;
  return data;
}

void makeEmptyStack()
{
  memoryInit();
  currentPosition = -1;
}

bool isEmptyStack()
{
  if(currentPosition == -1)
    return true;
  else
    return false;
}

int peek()
{
  return memory[currentPosition].data.queueData;
}

