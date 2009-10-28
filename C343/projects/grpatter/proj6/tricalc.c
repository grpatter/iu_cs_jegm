// Project 6, C343: Data Structures, Indiana University
// File: tricalc.c
// Name: Greg Patterson, grpatter
// Date: 10/26/2009
 
//--------------------------------------------
// PROJECT 2 SOURCE: Michael Adams
//--------------------------------------------
 
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
    Data temp = pop();
    if(temp.tag == false){
      printStack();
      int l = temp.length;
      for(int i = 0; i < l; i++){
    printf("%d",temp.queueData.term[i].coeff);
    if(temp.queueData.term[i].X > 0){
      printf("x^%d",temp.queueData.term[i].X);
    }
    if(temp.queueData.term[i].Y > 0){
      printf("y^%d",temp.queueData.term[i].Y);
    }
    if(temp.queueData.term[i].Z > 0){
      printf("z^%d",temp.queueData.term[i].Z);
    }
    printf(" + ");
      }
      printf("\n");
    }else{}
    push(temp);
  }
}
  
 
void smallStackError(char *input) {
  printf("Ignoring '%s': Stack too small\n", input);
}
 
void trinomialOperationError(char *input) {
  printf("Ignoring '%s': Can not be applied to Trinomials\n", input);
}

Data createEmptyData(){
	Data temp;
	temp.tag = false;
	temp.queueData.term[0].X = 0;
	temp.queueData.term[0].Y = 0;
	temp.queueData.term[0].Z = 0;
	temp.queueData.term[0].coeff = 0;
	temp.queueData.term[0].nextTerm = NIL;
	temp.length = 0;
	return temp;
}

void addHandler(){
	Data a = pop();
	Data b = pop();
	if (a.tag == false || b.tag == false) {
		b.queueData.term[b.length - 1].nextTerm = b.length;
		b.queueData.term[b.length] = a.queueData.term[a.length - 1];
		printf("setting coeff from a.length-1:%d in b at b.length:%d\n",a.queueData.term[a.length-1].coeff, b.length);
		printf("val coeff set to:%d\n",b.queueData.term[b.length].coeff);
		b.length++;
		push(b);
		Data bah = peek();
		for(int i = 0; i < bah.length; i++){
			printf("bahs coeff:%d\n",bah.queueData.term[i].coeff);
		}
	} else {
		Data temp;
		temp.tag = true;
		temp.queueData.stackData = b.queueData.stackData + a.queueData.stackData;
		push(temp);
	}
}
 
void subHandler(){
        // subtract is non-commutative so we can't inline the pops
        Data a = pop();//a
        Data b = pop();//b
    if(a.tag == false || b.tag == false) {
      
      b.queueData.term[b.length-1].nextTerm = b.length++;
      b.queueData.term[b.length-1] = a.queueData.term[a.length-1];
      b.queueData.term[b.length-1].coeff *= -1;
      push(b);
    } else {
      Data temp;
      temp.tag = true;
      temp.queueData.stackData = b.queueData.stackData - a.queueData.stackData;
      push(temp);
    }
}
 
void multHandler(){
    Data a = pop();
    Data b = pop();
    if ( a.tag == false || b.tag == false ) {
      b.queueData.term[b.length - 1].coeff *= a.queueData.term[a.length - 1].coeff;
      b.queueData.term[b.length - 1].X += a.queueData.term[a.length - 1].X;
      b.queueData.term[b.length - 1].Y += a.queueData.term[a.length - 1].Y;
      b.queueData.term[b.length - 1].Z += a.queueData.term[a.length - 1].Z;      
      push(b);      
    } else {
      Data temp;
      temp.tag = true;
      temp.queueData.stackData = a.queueData.stackData * b.queueData.stackData;
      push(temp);
    }
}
 
void divHandler(){
        // divide is non-commutative so we can't inline the pops
        Data a = pop();
        Data b = pop();
    if (a.tag == false || b.tag == false) {
      trinomialOperationError(input);
    } else if (a.queueData.stackData == 0) {
          printf("Ignoring illegal division by zero\n");
          push(b);
          push(a);
        } else {
      Data temp;
      temp.tag = true;
      temp.queueData.stackData = b.queueData.stackData / a.queueData.stackData;
          push(temp);
        }
}
 
int main() { 
  char input[101]; // max input is 100 plus 1 for '\0' char
  bool done = false;
  bool negate = false;
 
  initStack();
  
  while (!done) {
    if (scanf("%100s", input) == EOF) {
      done = true;
    } else if (isnumber(input)) {//create new data
		Data temp = createEmptyData();
		int n = atoi(input);
		if (negate) {
			n = n * -1;
		} else { }
		temp.queueData.term[0].coeff = n;
		temp.length = 1;
		push(temp);
    } else if (strcmp(input, "+") == 0) {//add
		if (length() < 2) {
			smallStackError(input);
		} else {
			addHandler();
		}
    } else if (strcmp(input, "-") == 0) {//sub
		if (length() < 2) {
			smallStackError(input);
		} else {
			subHandler();
		}
    } else if (strcmp(input, "*") == 0) {//mult
		if (length() < 2) {
			smallStackError(input);
		} else {
			multHandler();
		}
    } else if (strcmp(input, "/") == 0) {
      // integer divide
      if (length() < 2) {
        smallStackError(input);
      } else {
		divHandler();
      }
    } else if (strcmp(input, "|") == 0) {
      // bitwise or
      if (length() < 2) {
        smallStackError(input);
      } else {
    Data a = pop();
    Data b = pop();
    if (a.tag == false || b.tag == false) {
      trinomialOperationError(input);
    }else {
      Data temp;
      temp.tag = true;
      temp.queueData.stackData = a.queueData.stackData | b.queueData.stackData;
      push(temp);
    }
      }
    }else if (strcmp(input, "^") == 0) {
      // bitwise or
      if (length() < 2) {
        smallStackError(input);
      }else {
    Data a = pop();
    Data b = pop();
    if (a.tag == false || b.tag == false) {
      //TODO
      if (b.queueData.term[b.length - 1].X > 0) {
        b.queueData.term[b.length - 1].X = a.queueData.term[a.length - 1].coeff;
      } else if (b.queueData.term[b.length - 1].Y > 0) {
        b.queueData.term[b.length - 1].Y = a.queueData.term[a.length - 1].coeff;
      } else if (b.queueData.term[b.length - 1].Z > 0) {
        b.queueData.term[b.length - 1].Z = a.queueData.term[a.length - 1].coeff;
      }
      push(b);
    } else {
      Data temp;
      temp.tag = true;
      temp.queueData.stackData = pow((int)b.queueData.stackData, (int)a.queueData.stackData);
      push(temp);
    }
      }
    } else if (strcmp(input, "X") == 0) {
      Data temp;
      temp.queueData.term[0].X = 1;
      temp.queueData.term[0].Y = 0;
      temp.queueData.term[0].Z = 0;
      temp.queueData.term[0].coeff = 1;
      temp.queueData.term[0].nextTerm = NIL;
      temp.tag = false;
      temp.length = 1;
      push(temp);
    } else if (strcmp(input, "Y") == 0) {
      Data temp;
      temp.queueData.term[0].X = 0;
      temp.queueData.term[0].Y = 1;
      temp.queueData.term[0].Z = 0;
      temp.queueData.term[0].coeff = 1;
      temp.queueData.term[0].nextTerm = NIL;
      temp.tag = false;
      temp.length = 1;
      push(temp);
    } else if (strcmp(input, "Z") == 0) {
      Data temp;
      temp.queueData.term[0].X = 0;
      temp.queueData.term[0].Y = 0;
      temp.queueData.term[0].Z = 1;
      temp.queueData.term[0].coeff = 1;
      temp.queueData.term[0].nextTerm = NIL;
      temp.tag = false;
      temp.length = 1;
      push(temp);
    } else if (strcmp(input, "p") == 0) {
      // print whole stack
      printStack();
      printf("\n");
    } else if (strcmp(input, "t") == 0) {
      // print top of stack
      if (length() < 1) {
        smallStackError(input);
      } else {
    Data datum = peek();
    if (datum.tag == true) {
      printf("%d\n", datum.queueData.stackData);
    } else {
      //TODO
    }
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
  return 1;
}
