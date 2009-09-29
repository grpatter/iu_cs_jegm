// Project 2, C343: Data Structures, Indiana University
// File: linkedCalculator.c
// Name: Michael Adams
// Username: adamsmd
// Date: 2009 Sept.

#include "stack.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool isnumber(char *str) {
  bool result = true;
  for (int i = 0; str[i] != '\0'; i++) {
    result = result && isdigit(str[i]);
  }
  return result;
}

void printStack() {
  if (!isEmpty()) {
	QData temp = pop();
    printStack();
	if(temp.tag == 0){
		printf("%d\n", t.myunion.num);
	}else if(temp.tag == 1){
		printf("%c\n", t.myunion.var);
	}else{
		printf("%c\n", t.myunion.op);
	}
    push(temp);
  }
}

void smallStackError(char *input) {
  printf("Ignoring '%s': Stack too small\n", input);
}

int main() {

  char input[101]; // max input is 100 plus 1 for '\0' char
  bool done = false;

  initStack();

  while (!done) {
    // the "%100s" tells scanf we want no more than 100 chars
    if (scanf("%100s", input) == EOF) {
      // End of input, assume the user wants to exit
      done = true;
    } else if (isnumber(input)) {
      // push number on stack
      push(atoi(input));
    } else if (strcmp(input, "+") == 0) {
      // integer add
      if (length() < 2) {
        smallStackError(input);
      } else {
        push(pop() + pop());
      }
    } else if (strcmp(input, "-") == 0) {
      // integer subtract
      if (length() < 2) {
        smallStackError(input);
      } else {
        // subtract is non-commutative so we can't inline the pops
        int a = pop();
        int b = pop();
        push(b - a);
      }
    } else if (strcmp(input, "*") == 0) {
      // integer multiply
      if (length() < 2) {
        smallStackError(input);
      } else {
        push(pop() | pop());
      }
    } else if (strcmp(input, "/") == 0) {
      // integer divide
      if (length() < 2) {
        smallStackError(input);
      } else {
        // divide is non-commutative so we can't inline the pops
        int a = pop();
        int b = pop();
        if (a == 0) {
          printf("Ignoring illegal division by zero\n");
          push(b);
          push(a);
        } else {
          push(b / a);
        }
      }
    } else if (strcmp(input, "|") == 0) {
      // bitwise or
      if (length() < 2) {
        smallStackError(input);
      } else {
        push(pop() | pop());
      }
    }else if (strcmp(input, "^") == 0){
		//exponentiation
		if(length() < 2){
			smallStackError(input);
		}else{
			if(peek() >= 0){
				int n = pop();//loop #
				int base = pop();//init value
				base = pow(base, n);
				push(base);
			}
		}
	}else if (strcmp(input, "p") == 0) {
      // print whole stack
      printStack();
      printf("\n");
    } else if (strcmp(input, "t") == 0) {
      // print top of stack
      if (length() < 1) {
        smallStackError(input);
      } else {
        printf("%d\n", peek());
      }
    } else if (strcmp(input, "w") == 0) {
      // wipe stack
      while (!isEmpty()) {
        pop();
      }
    } else if (strcmp(input, "q") == 0) {
      // quit
      done = true;
    } else {
      printf("Ignoring unknown command: %s\n", input);
    }
  }

  return 0;
}
