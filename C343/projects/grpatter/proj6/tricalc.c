// Project 3, C343: Data Structures, Indiana University
// File: linkedCalculator.c
// Name: Greg Patterson
// Username: grpatter
// Credit: Michael Adams
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

bool isvariable(char *str){
	bool result = false;
	if(strcmp(str, "X") == 0 || strcmp(str, "Y") == 0 || strcmp(str, "Z") == 0){
		result = true;
		//printf("found variable\n");
	}
	return result;
}

void printStack() {
  if (!isEmpty()) {
	Data temp = pop();
    printStack();
	if(temp.tag == 0){
		printf("%d ", temp.myunion.num);
	}else if(temp.tag == 1){
		printf("%c ", temp.myunion.var);
	}else{
		printf("%c ", temp.myunion.op);
	}
    push(temp);
  }
}

void smallStackError(char *input) {
  printf("Ignoring '%s': Stack too small\n", input);
}

Data qd_create_int(int n){
	Data qd;
	qd.tag = 0;
	qd.myunion.num = n;
	return qd;
}
Data qd_create_char(char n, int type){
	//printf("create_char got:%c\n", n);
	Data qd;
	if(type == 1){
		qd.tag = 1;
		qd.myunion.var = n;
	}else{
		qd.tag = 2;
		qd.myunion.op = n;
	}
	return qd;
}

bool check_idents(Data p1, Data p2, char op){
	//printf("checking idents for:%c\n", op);
	bool reduc = false;
	bool annih = false;
	int oper = 0; //+ 1, -2, * 3, / 4
	if(op == '+'){
		//printf("opn matches + \n");
		oper = 1;
	}else if(op == '-'){
		oper = 2;
	}else if(op == '*'){
		oper = 3;
	}else if(op == '/'){
		oper = 4;
	}else if(op == '^'){
		oper = 5;
	}else{
		return reduc;
	}
	//printf("oper is:%d\n",oper);
	switch(oper){
		case 1://+reduc
			//printf("case + checking rules\n");
			if(p1.tag == 0 && p1.myunion.num == 0){
				//can reduce p2 to its value
				push(p2);//add it back to stack, all done
				reduc = true;
			}else if(p2.tag == 0 && p2.myunion.num == 0){
				push(p1);
				reduc = true;
			}else{
				push(p2);
				push(p1);
				reduc = false;
			}
			break;
		case 2://-reduc
			//printf("case - checking rules\n");
			if(p2.tag == 0 && p2.myunion.num == 0){
			//printf("case - matched a rule\n");
				push(p1);
				reduc = true;
			}else{
				push(p2);
				push(p1);
				reduc = false;
			}
			break;
		case 3://*reduc
			//printf("case * checking rules\n");
			if(p1.tag == 0 && p1.myunion.num == 1){
				//can reduce p2 to its value				
				push(p2);//add it back to stack, all done
				reduc = true;
			}else if(p2.tag == 0 && p2.myunion.num == 1){
				push(p1);
				reduc = true;
			}else if(p1.tag == 0 && p1.myunion.num == 0){//annih
				Data temp = qd_create_int(0);
				push(temp);
				annih = true;
			}else if(p2.tag == 0 && p2.myunion.num == 0){//annih
				Data temp = qd_create_int(0);
				push(temp);
				annih = true;
			}else{
				push(p2);
				push(p1);
				reduc = false;
				annih = false;
			}
			break;
		case 4:// / reduc
			//printf("case / checking rules\n");
			if(p2.tag == 0 && p2.myunion.num == 1){
				push(p1);
				reduc = true;
			}else if(p1.tag == 0 && p1.myunion.num == 0){//annih
				//printf("matching / annih rule \n");
				Data temp = qd_create_int(0);
				//printf("temp val: %d\n", temp.myunion.num);
				push(temp);
				annih = true;
			}else{
				push(p2);
				push(p1);
				reduc = false;
				annih = false;
			}
			break;
		case 5://^annih
			//printf("case ^ checking rules\n");
			if(p1.tag == 0 && p1.myunion.num == 0){
				//printf("matching ^ annih rule \n");
				Data temp = qd_create_int(1);
				//printf("temp val: %d\n", temp.myunion.num);
				push(temp);
				annih = true;
			}else{
				push(p2);
				push(p1);
				annih = false;
			}
			break;
		default: break;
	}
	bool rule_applied = false;
	if(reduc){
		rule_applied = true;
		printf("Reduction Rule Applied.\n");
	}
	if(annih){
		rule_applied = true;
		printf("Annihilation Rule Applied.\n");
	}
	return rule_applied;
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
	  Data tempD = qd_create_int(atoi(input));
      push(tempQD);
    } else if (isvariable(input)){
		//push var on stack
		Data tempQD = qd_create_char(input[0], 1);
		//printf("adding var to stack\n");
		push(tempQD);
	}else if (strcmp(input, "+") == 0) {
      // integer add
      if (length() < 2) {
        smallStackError(input);
      } else {
        //push(pop() + pop());
		bool c = true;
		while(c){
			Data p1 = pop();
			Data p2 = pop();
			if(p1.tag == 0 && p2.tag == 0){
				//printf("adding data p1 p2\n");
				int t = p1.myunion.num + p2.myunion.num;
				Data tempQD = qd_create_int(t);
				push(tempQD);
				c = false;
			}else{
				c = false;
				check_idents(p1, p2, '+');
				//if(check_annihs(p1, p2, '+')){ }
			}
		}
	  }
    } else if (strcmp(input, "-") == 0) {
      // integer subtract
      if (length() < 2) {
        smallStackError(input);
      } else {
        // subtract is non-commutative so we can't inline the pops
		bool c = true;
		while(c){
			Data p1 = pop();
			Data p2 = pop();
			if(p1.tag == 0 && p2.tag == 0){
				int t = p2.myunion.num - p1.myunion.num;
				Data tempQD = qd_create_int(t);
				push(tempQD);
				c = false;
			}else{
				c = false;
				check_idents(p2, p1, '-');
				//if(check_annihs(p2, p1, '-')){}
			}
		}
      }
    } else if (strcmp(input, "*") == 0) {
      // integer multiply
      if (length() < 2) {
        smallStackError(input);
      } else {
        //push(pop() | pop());
		bool c = true;
		while(c){
			Data p1 = pop();
			Data p2 = pop();
			if(p1.tag == 0 && p2.tag == 0){
				int t = p2.myunion.num * p1.myunion.num;
				Data tempQD = qd_create_int(t);
				push(tempQD);
				c = false;
			}else{
				check_idents(p1, p2, '*');
				//check_annihs(p1, p2, '*');
				c = false;
			}
		}
      }
    } else if (strcmp(input, "/") == 0) {
      // integer divide
      if (length() < 2) {
        smallStackError(input);
      } else {
        // divide is non-commutative so we can't inline the pops
		bool c = true;
		while(c){
			Data p1 = pop();
			Data p2 = pop();
			if(p1.tag == 0 && p2.tag == 0){
				if(p1.myunion.num == 0){
					printf("Ignoring illegal division by zero\n");
					push(p2);
					push(p1);
				}else{
				int t = p2.myunion.num / p1.myunion.num;
				Data tempQD = qd_create_int(t);
				push(tempQD);
				c = false;
				}
			}else{
				c = false;
				check_idents(p2, p1, '/');
				//if(check_annihs(p2, p1, '\\')){}
			}
		}
      }
    } else if (strcmp(input, "|") == 0) {
      // bitwise or
      if (length() < 2) {
        smallStackError(input);
      } else {
        //push(pop() | pop());
		Data p1 = pop();
		Data p2 = pop();
		int t = p1.myunion.num | p2.myunion.num;
		Data tempQD = qd_create_int(t);
		push(tempQD);
      }
    }else if (strcmp(input, "^") == 0){
		//exponentiation
		if(length() < 2){
			smallStackError(input);
		}else{
				Data n = pop();
				Data base = pop();
				if(n.tag == 0 && base.tag == 0){
					int t = pow(base.myunion.num, n.myunion.num);
					Data tempQD = qd_create_int(t);
					push(tempQD);
				}else{
					check_idents(n, base, '^');
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
        Data temp = peek();
		if(temp.tag == 0){
			printf("%d\n", temp.myunion.num);
		}else if(temp.tag == 1){
			printf("%c\n", temp.myunion.var);
		}else{
			printf("%c\n", temp.myunion.op);
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

  return 0;
}
