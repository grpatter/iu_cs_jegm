/*
** Jonathan M. Stout
** stack.c
*/

#include <stdio.h>
#define MEMSIZE 2048;

int TOP;
info mem[MEMSIZE];

void push(info datum)
{
  if( (++TOP) >= MEMSIZE )
    OVERFLOW();
  else
    mem[TOP] = datum;
  return ;
}

info pop()
{
  info ans = peek();
  
  if ( isEmptyStack() )
    UNDERFLOW();
  else
    return mem[TOP--];
}

void makeEmptyStack()
{
  TOP = -1;
  return; //Space is void
}

bool isEmptyStack()
{
  return TOP == -1;
}

info peek()
{
  if ( isEmptyStack() )
    UNDERFLOW();
  else
    return mem[top];
}
