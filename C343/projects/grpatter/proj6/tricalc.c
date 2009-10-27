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

int termcount = 0;

bool isnumber(char *str);
bool isvariable(char *str);
void adjust_exp(int n, int vcount, char last_var);
void printStack();
void print_trig_vals(Data tmp, int a);
void smallStackError(char *input);
bool check_idents(Data p1, Data p2, char op);
void add_new_term(int c, int vcount);
void adjust_exp(int n, int vcount, char last_var);
void adjust_sign(int vcount);

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
		print_trig_vals(temp, 0);	
		if(temp.vals[0].term.nextterm != 0){
			print_trig_vals(temp, 1);
		}
		if(temp.vals[1].term.nextterm != 0){
			print_trig_vals(temp, 2);
		}
	}else{
		printf("%d ", temp.vals[0].stackdata.coeff);
	}
    push(temp);
  }
}

void print_trig_vals(Data tmp, int a){
	printf("%d",tmp.vals[a].term.coeff);
	if(tmp.vals[a].term.x != 0){
		printf("X^%d",tmp.vals[a].term.x);	
	}
	if(tmp.vals[a].term.y != 0){
		printf("Y^%d",tmp.vals[a].term.y);		
	}
	if(tmp.vals[a].term.z != 0){
		printf("Z^%d",tmp.vals[a].term.z);		
	}
	printf(" ");
}

void smallStackError(char *input) {
  printf("Ignoring '%s': Stack too small\n", input);
}


/*
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
*/

void add_new_term(int c, int vcount){
	Data d;
	d.tag = 0;//term
	d.vals[vcount].term.coeff = c;	
	d.vals[vcount].term.x = 0;	
	d.vals[vcount].term.y = 0;	
	d.vals[vcount].term.z = 0;
	push(d);
	termcount++;
}

void adjust_sign(int vcount){
Data d = pop();
d.vals[vcount].term.coeff = d.vals[vcount].term.coeff * -1;
push(d);
}

void adjust_exp(int n, int vcount, char last_var){
Data d = pop();
if(last_var = 'X'){
	d.vals[vcount].term.x = n;
	printf("adjusted exp x to %d...\n", d.vals[vcount].term.x = n);
}else if(last_var = 'Y'){
	d.vals[vcount].term.y = n;
	printf("adjusted exp y to %d...\n", d.vals[vcount].term.y = n);
}else if(last_var = 'Z'){
	d.vals[vcount].term.z = n;
	printf("adjusted exp z to %d...\n", d.vals[vcount].term.z = n);
}else{
}
push(d);
}

void clearFields(){
}

int main() {
  char input[101]; // max input is 100 plus 1 for '\0' char
  bool done = false;
  bool in_tri = false;
  int vcount = 0;
  char last_var = ".";

  initStack(); 
  clearFields();
  
  while (!done) {
	if(vcount >= 3){
		printf("Resetting flags...\n");
		vcount = 0;
		last_var = ".";
		in_tri = false;
	}
    if (scanf("%100s", input) == EOF) {
      done = true;
    } else if (isnumber(input)) {
		if(!in_term){
		  printf("Not in_tri, adding new term...\n");
		  add_new_term(atoi(input), vcount);
		}else{//must be exponent val
			printf("In_tri, adjusting exp...\n");
		  adjust_exp(atoi(input), vcount, last_var);
		}
    } else if (isvariable(input)){
		in_tri = true;
		last_var = input[0];
		printf("Is a variable, saving last_var as: %c...\n", last_var);
	}else if (strcmp(input, "+") == 0) {
		vcount++;
    } else if (strcmp(input, "-") == 0) {
		adjust_sign(vcount);
		vcount++;
    } else if (strcmp(input, "*") == 0) {
		//
    }else if (strcmp(input, "^") == 0){
		//hm
	}else if (strcmp(input, "p") == 0) {
      printStack();
      printf("\n");
    } else if (strcmp(input, "t") == 0) {
		Data temp = pop();
		if(temp.tag){
			print_trig_vals(temp, 0);	
			if(temp.vals[0].term.nextterm != 0){
				print_trig_vals(temp, 1);
			}
			if(temp.vals[1].term.nextterm != 0){
				print_trig_vals(temp, 2);
			}
		}else{
			printf("%d ", temp.vals[0].stackdata.coeff);
		}
		push(temp);
    } else if (strcmp(input, "w") == 0) {
      while (!isEmpty()) {
        pop();
      }
    } else if (strcmp(input, "q") == 0) {
      done = true;
    } else {
      printf("Ignoring unknown command: %s\n", input);
    }
  }
  return 0;
}
