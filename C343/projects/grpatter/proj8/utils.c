/* File: utils.c
 * Greg Patterson grpatter
 */
#include "utils.h"

void flush_io() {
  fflush(stdin);
  fflush(stdout);
}

bool isnumber(char *str) {
  bool result = true;
  for (int i = 0; str[i] != '\0'; i++) {
    result = result && isdigit(str[i]);
  }
  return result;
}

bool isEven(int num) {
  if(num % 2 == 0) {
    return true;
  } else {
    return false;
  }
}

int minimum(int a, int b) {
  if(a < b) {
    return a;
  } else {
    return b;
  }
}
