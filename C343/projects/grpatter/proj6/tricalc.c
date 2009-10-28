// Project 6, C343: Data Structures, Indiana University
// File: tricalc.c
// Name: Greg Patterson, grpatter
// Date: 10/27/2009
// Credit: Mchael Adams
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stack.h"

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
	}
	return result;
}

void printTerm(Data temp){
	int l = temp.length;
	for (int i = 0; i < l; i++) {
		printf("%d", temp.jData.term[i].coeff);
		if (temp.jData.term[i].X > 0) {
			printf("X^%d", temp.jData.term[i].X);
		}
		if (temp.jData.term[i].Y > 0) {
			printf("Y^%d", temp.jData.term[i].Y);
		}
		if (temp.jData.term[i].Z > 0) {
			printf("Z^%d", temp.jData.term[i].Z);
		}
		if(i+1 < l){
			printf(" + ");
		}
	}
}

void printStack() {
	if (!isEmpty()) {
		Data temp = pop();
		if (temp.tag == false) {
			printStack();
			printTerm(temp);
			printf("\n");
		} else {
		}
		push(temp);
	}
}

void UNDERFLOW(char *input) {
	printf("UNDERFLOW. Stack too small for request[%s], Ignoring...\n", input);
}

void invalidRequest(char *input) {
	printf("Request[%s] cannot be performed with given arguments, Ignoring...\n", input);
}

Data createEmptyData() {
	Data temp;
	temp.tag = false;
	temp.jData.term[0].X = 0;
	temp.jData.term[0].Y = 0;
	temp.jData.term[0].Z = 0;
	temp.jData.term[0].coeff = 0;
	temp.jData.term[0].nxTerm = NIL;
	temp.length = 0;
	return temp;
}

void addHandler() {
	Data two = pop();
	Data one = pop();
	if (two.tag == false || one.tag == false) {
		while(two.length > 0){
			one.jData.term[one.length - 1].nxTerm = one.length;
			one.jData.term[one.length] = two.jData.term[two.length - 1];
			one.length++;
			two.length--;
		}
		push(one);
	} else {
		Data temp;
		temp.tag = true;
		temp.jData.stackData = one.jData.stackData
				+ two.jData.stackData;
		push(temp);
	}
}

void subHandler() {
	Data two = pop();
	Data one = pop();
	if (two.tag == false || one.tag == false) {
		while(two.length > 0){
			one.jData.term[one.length - 1].nxTerm = one.length++;
			one.jData.term[one.length - 1] = two.jData.term[two.length - 1];
			one.jData.term[one.length - 1].coeff *= -1;
			two.length--;
		}
		push(one);
	} else {
		Data temp;
		temp.tag = true;
		temp.jData.stackData = one.jData.stackData
				- two.jData.stackData;
		push(temp);
	}
}

void multHandler() {
	Data two = pop();
	Data one = pop();
	if (two.tag == false || one.tag == false) {
		one.jData.term[one.length - 1].coeff
				*= two.jData.term[two.length - 1].coeff;
		one.jData.term[one.length - 1].X += two.jData.term[two.length - 1].X;
		one.jData.term[one.length - 1].Y += two.jData.term[two.length - 1].Y;
		one.jData.term[one.length - 1].Z += two.jData.term[two.length - 1].Z;
		push(one);
	} else {
		Data temp;
		temp.tag = true;
		temp.jData.stackData = two.jData.stackData
				* one.jData.stackData;
		push(temp);
	}
}

void divHandler() {
	Data two = pop();
	Data one = pop();
	if (two.tag == false || one.tag == false) {
		//invalidRequest(input);
	} else if (two.jData.stackData == 0) {
		printf("Division by zero, ignoring...\n");
		push(one);
		push(two);
	} else {
		Data temp;
		temp.tag = true;
		temp.jData.stackData = one.jData.stackData
				/ two.jData.stackData;
		push(temp);
	}
}

void expHandler(){
	Data a = pop();
	Data b = pop();
	if (a.tag == false || b.tag == false) {
		if (b.jData.term[b.length - 1].X > 0) {
			b.jData.term[b.length - 1].X
					= a.jData.term[a.length - 1].coeff;
		} else if (b.jData.term[b.length - 1].Y > 0) {
			b.jData.term[b.length - 1].Y
					= a.jData.term[a.length - 1].coeff;
		} else if (b.jData.term[b.length - 1].Z > 0) {
			b.jData.term[b.length - 1].Z
					= a.jData.term[a.length - 1].coeff;
		}
		push(b);
	} else {
		Data temp;
		temp.tag = true;
		temp.jData.stackData = pow((int) b.jData.stackData,
				(int) a.jData.stackData);
		push(temp);
	}
}

void wipeStack(){
while (!isEmpty()) {
	pop();
}
}

bool isUNDERFLOW(char *input){
	if(length() < 2){
		UNDERFLOW(input);
		return true;
	}else{
		return false;
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
			}
			temp.jData.term[0].coeff = n;
			temp.length = 1;
			push(temp);
		} else if (strcmp(input, "+") == 0) {//add
			if (!isUNDERFLOW(input)){
				addHandler();
			}
		} else if (strcmp(input, "-") == 0) {//sub
			if (!isUNDERFLOW(input)){
				subHandler();
			}
		} else if (strcmp(input, "*") == 0) {//mult
			if (!isUNDERFLOW(input)){
				multHandler();
			}
		} else if (strcmp(input, "/") == 0) {//div
			if (!isUNDERFLOW(input)){
				divHandler();
			}
		} else if (strcmp(input, "|") == 0) {
			printf("Not handling | operation. Ignoring...\n");
		} else if (strcmp(input, "^") == 0) {//power
			if (!isUNDERFLOW(input)){
				expHandler();
			}
		} else if(isvariable(input)){
			if (strcmp(input, "X") == 0) {//should use a switch or outside function
				Data temp = createEmptyData();
				temp.jData.term[0].X = 1;
				temp.jData.term[0].coeff = 1;
				temp.length = 1;
				push(temp);
			} else if (strcmp(input, "Y") == 0) {
				Data temp = createEmptyData();
				temp.jData.term[0].Y = 1;
				temp.jData.term[0].coeff = 1;
				temp.length = 1;
				push(temp);
			} else if (strcmp(input, "Z") == 0) {
				Data temp = createEmptyData();
				temp.jData.term[0].Z = 1;
				temp.jData.term[0].coeff = 1;
				temp.length = 1;
				push(temp);
			}
		}else if (strcmp(input, "p") == 0) {
			printStack();
			printf("\n");
		} else if (strcmp(input, "t") == 0) {
			if (length() < 1) {
				UNDERFLOW(input);
			} else {
				Data temp = peek();
				if (temp.tag == true) {//stackdata
					printf("%d\n", temp.jData.stackData);
				} else {//term					
					printTerm(temp);
				}
			}
		} else if (strcmp(input, "w") == 0) {
			wipeStack();
		} else if (strcmp(input, "q") == 0) {
			done = true;
		} else {
			printf("Ignoring unknown command: %s\n", input);
		}
	}
	return 1;
}
