/*
** grpatter
** calculator.c
** 
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"

#define EQNLEN 100;


int main(){
	makeEmptyStack();//create the stack to use
	printf("Please enter the equation to be evaluated.\n");
	char str[100];//to store the eqn
  
	while (1){	//loop indef.
		scanf("%s", &str);//read until next white space
		if((*str) == 'q'){ //stop now and get out.
			exit(0);
		}
		int stuff = atoi(str);//get integer value of the input
		if ((*str) == 't'){
			printf("%d\n",peek());//display result from peek op
		}else if ((*str) == 'p'){//print the stack
			printstack();
		}else if((*str) == 'w'){//delete the stack
			wipe_stack();
		}else if((*str) == '+' ||(*str) == '-'||(*str) == '*' ||(*str) == '/' ){
			ops(&str);//go to ops handler
		}else {
			push(stuff);//this is an int, add it to the stack
			//printf("%d:pushed to stack\n",stuff);
		}
	}
}

void ops(char *str){
	if((*str) == '+'){
		//printf("adding op\n");
		int arg1 = pop();
		int arg2 = pop();
		int res = arg1 + arg2;
		push(res);
	}else if((*str) == '-'){
		//printf("sub op\n");
		int arg1 = pop();
		int arg2 = pop();
		int res = arg2 - arg1;
		push(res);
	}else if((*str) == '*'){
		//printf("mul op\n");
		int arg1 = pop();
		int arg2 = pop();
		int res = arg1 * arg2;
		push(res);
	}else if((*str) == '/'){
		//printf("div op\n");
		int arg1 = pop();
		int arg2 = pop();
		int res = arg2 / arg1;
		push(res);
	}
	return;
}


