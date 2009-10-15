/*
** File: linkedStack.c
** Name: Jonathan M. Stout | jonstout@indiana.edu
** Date: 09/17/2009
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "AVAIL.h"
#include "stack.h"

Pointer currentPosition;

void push(QueueData datum)
{
  currentPosition = fromAVAIL();
  memory[currentPosition].data.queueData = datum;
}

QueueData pop()
{
  QueueData data =  memory[currentPosition].data.queueData;
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

QueueData peek()
{
  if(memory[currentPosition].data.queueData.tag == 0)
    return memory[currentPosition].data.queueData;
  else
    return memory[currentPosition].data.queueData;
}

