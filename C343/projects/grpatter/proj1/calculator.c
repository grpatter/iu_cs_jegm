#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"

#define EQNLEN 100;
int main()
{
  makeEmptyStack();

  printf("Please enter the equation to be evaluated.\n");
  char str[100];
  
while (1){	
	scanf("%s", &str);
	if((*str) == 'q'){ exit(0);}
	int stuff = atoi(str);
	if ((*str) == 't'){
		printf("%d\n",peek());
	}else if ((*str) == 'p'){
		printstack();
	}else if((*str) == '+'){
		printf("adding op\n");
		int arg1 = pop();
		int arg2 = pop();
		int res = arg1 + arg2;
		push(res);
	}else if((*str) == '-'){
		printf("sub op\n");
		int arg1 = pop();
		int arg2 = pop();
		int res = arg1 - arg2;
		push(res);
	}else if((*str) == '*'){
		printf("mul op\n");
		int arg1 = pop();
		int arg2 = pop();
		int res = arg1 * arg2;
		push(res);
	}else if((*str) == '/'){
		printf("div op\n");
		int arg1 = pop();
		int arg2 = pop();
		int res = arg1 / arg2;
		push(res);
	}else {
		push(stuff);
		printf("%d:pushed to stack\n",stuff);
	}
}
	printf("%s", &str);
}
