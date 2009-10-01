// Project 2, C343: Data Structures, Indiana University
// File: stack.h
// Name: Greg Patterson
// Username: grpatter
// Credit: Michael Adams
// Date: 2009 Sept.

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "AVAIL.h"

void initStack();
bool isEmpty();
int length();
void push(QData value);
QData pop();
QData peek();

#endif // STACK_H
