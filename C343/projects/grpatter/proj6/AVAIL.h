/* File: AVAIL.h
 * Copyright (c) 2004, 2007 and 2009, David S. Wise and Michael D. Adams
 */

#ifndef AVAIL_H
#define AVAIL_H

#include <assert.h>  // for assert()
#include <stdbool.h> // for bool, true and false
#include <stdio.h>   // for printf()
#include <stdlib.h>  // for exit()

// The "pointer" type
typedef int Pointer;

//-------- The user should change the "Data" type as needed --------
typedef struct {
  int coeff;
  int X;
  int Y;
  int Z;
  Pointer nxTerm;
} Term;

typedef int StackData;
 
typedef union {
  Term term[100];
  StackData stackData;
} joinData;
 
typedef struct {
  bool tag;
  int length;
  joinData jData;
  Pointer next;
} Data;
//-------- Do not change code below here --------

// The null pointer.  (Use this instead of NULL.)
#define NIL ((Pointer)-1)

typedef union {
  Pointer nextFree; // Should only ever be used by AVAIL.c
  Data data;
} Cell;

// The memory heap
extern Cell memory[];

// Initialize the memory.
// Must be called before fromAVAIL() and toAVAIL()
// Must only be called once.
void memoryInit();

// Allocate a Pointer from free memory.
Pointer fromAVAIL();

// Return a Pointer back to free memory.
void toAVAIL(Pointer p);

#endif /* AVAIL_H */
