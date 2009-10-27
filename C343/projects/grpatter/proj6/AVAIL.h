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
typedef struct{	
  int coeff;
  int x;
  int y;
  int z;
  Pointer nextterm;
} Term;
typedef struct{	
  int coeff;
  Pointer nextterm;
} Stackdata;
typedef struct {
  bool tag;//0 tri, 1 stackdata
  union{
	  Stackdata stackdata;
	  Term term;
  } vals[3];
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
