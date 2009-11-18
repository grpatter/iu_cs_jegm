/* File: cats.h
 * Greg Patterson grpatter
 */

#ifndef cats_H
#define cats_H

#define NIL -1
#define MAXCATS 500
#define DISTSIZE 50

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
  int generation;
  int tag;
  int sire;
  int sire_half_sib;
  int dam;
  int dam_half_sib;
  int eldest_c;
  int youngest_c;
} Cat;

typedef struct {
  int i;
  int j;
} Distance;

Cat tree[MAXCATS];
Distance d_store[DISTSIZE];

#endif /* cats_H */
