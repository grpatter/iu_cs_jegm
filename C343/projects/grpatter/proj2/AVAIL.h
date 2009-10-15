/* 
** avail.h
** Credit: Copyright 2002, 2004, 2009, David S. Wise and Michael D. Adams
** Greg Patterson (grpatter at indiana dot edu)
** 9/22/2009
*/


#ifndef AVAIL_H
#define AVAIL_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int Pointer;

typedef int QueueData;
typedef struct {
  QueueData queueData;
  Pointer next;
} Data;

#define NIL ((Pointer)-1)

typedef union {
  Pointer nextFree;
  Data data;
} Cell;

extern Cell memory[];

void memoryInit();

Pointer fromAVAIL();

void toAVAIL(Pointer p);

#endif
