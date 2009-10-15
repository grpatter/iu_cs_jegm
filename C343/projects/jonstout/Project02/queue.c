/*
** FILE: queue.c
** SRC : Copyright 2002, 2004, 2009, David S. Wise and Michael D. Adams
** NAME: Jonathan M. Stout | jonstout@indiana.edu
** Date: 09/21/2009
*/

#include "queue.h"
#include "AVAIL.h"

Queue q;

/* Public methods */

/* Set up an empty queue */
void queueInit(){
  memoryInit();  // from AVAIL
  (q.front) = NIL;
  return;
}

/* Test for an empty queue */
bool isEmpty()
{
  return (q.front == (Pointer)-1);
}

/* Add a value to a queue. */
void enQueue(QueueData value)
{
  Pointer newNode = fromAVAIL();
  memory[newNode].data.next = (Pointer)-1;
  memory[newNode].data.queueData = value;

  if ( isEmpty(q) )
    (q.front) = newNode;
  else
    memory[q.rear].data.next = newNode;
  (q.rear) = newNode;

  return;
}

/* Remove a value from a queue. */

QueueData fromQueue()
{
  if (isEmpty(q))
    {
      // In general you would try to recover here, but for now we have
      // no way to recover so we just print an error message and exit.
      exit(-1);
    } else {
    Pointer oldNode = q.front;
    QueueData value = memory[oldNode].data.queueData;
    q.front = memory[oldNode].data.next;

    toAVAIL(oldNode);
    return value;
  }
}
