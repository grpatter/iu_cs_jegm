/* File: utils.h
 * Greg Patterson grpatter
 * Credits:
 * isnumber(...) concepts: Michael Adams
 */

#ifndef utils_H
#define utils_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void flush_io();
bool isnumber(char *str);
bool isEven(int num);
int minimum(int a, int b);

#endif /* utils_H */
