/* File: AVAIL.c
 * Copyright (c) 2004, 2005, 2007 and 2009 by David S. Wise and Michael D. Adams
 */

#include "AVAIL.h"

// NOTE: MEMORY_SIZE should be defined on the
//  command line with: -DMEMORY_SIZE=<number-of-cells>
Cell memory[MEMORY_SIZE];

static Pointer freeList;
static bool initialized = false;

void memoryInit()
{
  assert(!initialized); // memoryInit() should only be called once

  // Set-up the freeList
  freeList = NIL;
  for (int i = MEMORY_SIZE-1; i >= 0; i--)
  {
    memory[i].nextFree = freeList;
    freeList = i;
  }

  initialized = true;
  return;
}

void memoryOverflow()
{
  // Until we have a garbage collector there is no way to repair
  // memory exhaustion, so just print an error message and quit.
  printf("ERROR: out of memory space\n");
  exit(-1);
  return;
}

Pointer fromAVAIL()
{
  assert(initialized);
  if (freeList == NIL)
    // Out of memory.  Try to recover.
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
