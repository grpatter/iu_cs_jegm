// Project 2, C343: Data Structures, Indiana University
// File: stack.h
// Name: Michael Adams
// Username: adamsmd
// Date: 2009 Sept.

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

void initStack();
bool isEmpty();
int length();
void push(int value);
int pop();
int peek();

#endif // STACK_H
