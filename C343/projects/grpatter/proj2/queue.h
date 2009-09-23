/* 
** queue.h
** Credit: Copyright 2002, 2004, 2009, David S. Wise and Michael D. Adams
** Greg Patterson (grpatter at indiana dot edu)
** 9/22/2009
*/


#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h> // for bool, true and false
#include <stdio.h>   // for printf()
#include <stdlib.h>  // for exit()

#ifndef SEQUENTIAL
#include "AVAIL.h"
#else
typedef int QueueData     //or whatever type is collected in the queue.
extern QueueData mem[];   // Somebody must declare the memory vector.
#endif

typedef struct
{
  // Private variables
  Pointer front;
  Pointer rear;
} Queue;

// Public methods

// Set up empty queue
void queueInit();

// Test for an empty queue
bool isEmpty();

// Add a value to the end of a queue
void enQueue(QueueData value);

// Remove a value from the front of a queue
QueueData fromQueue();

#endif /* QUEUE_H */
