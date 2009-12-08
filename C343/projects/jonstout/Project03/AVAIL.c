/*
** File: AVAIL.h
** Src : David S. Wise and Michael D. Adams
** Name: Jonathan M. Stout | jonstout@indiana.edu
** Date: 09/17/2009
*/

#include "AVAIL.h"

// NOTE: MEMORY_SIZE should be defined on the
// command line with: -MEMORY_SIZE=<number-of-cells>
Cell memory[MEMORY_SIZE];

static Pointer freeList;
static bool initialized = false;

void memoryInit()
{
  assert(!initialized); // memoryInit() should only be called once

  freeList = NIL;
  for (int i = MEMORY_SIZE-1; i >= 0; i--)
  {
    memory[i].nextFree = freeList;
    freeList = i;
  }

  initialized = true;
  return ;
}

void memoryOverflow()
{
  printf("ERROR: out of memory space\n");
  exit(-1);
  return;
}

Pointer fromAVAIL()
{  
  assert(initialized);
  if (freeList == NIL)
    memoryOverflow();
  
  Pointer p = freeList;
  freeList = memory[freeList].nextFree;
  return p;
}

void toAVAIL(Pointer p)
{
  assert(initialized);
  assert(p != NIL);
  memory[p].nextFree = freeList;
  freeList = p;
  return;
}

