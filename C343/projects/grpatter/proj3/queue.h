// Project 2, C343: Data Structures, Indiana University
// File: queue.h
// Name: Michael Adams
// Username: adamsmd
// Date: 2009 Sept.

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

void initQueue();
bool isEmptyQueue();
int lengthQueue();
void enQueue(int value);
int fromQueue();
int peekQueue();

#endif // QUEUE_H
