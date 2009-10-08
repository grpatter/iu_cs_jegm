/* 
** linkedCalculator.c
** Greg Patterson (grpatter at indiana dot edu)
** 9/22/2009
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"

void stack_ops(char *input);
void print_stack();

int main(){
  char equation[100];//to hold entered eqn.
  makeEmptyStack();//get us a stack
  printf("Please enter the equation to be evaluated.\n");

  while(1){
	scanf("%s", equation);//get eqn from stdin

	if(*equation == 'q'){//quit
		break;//get out
	}else if(*equation == 'p'){//print
		print_stack();//call print
	}else if(*equation == 't'){//peek val
		if(!isEmptyStack()){//verify we are not at end
			printf("%d\n", peek());//show val
		}//else ignore this
	}else if(*equation == 'w'){//wipe the stack
		while(!isEmptyStack()){//while not empty
			pop();//keep popping vals off
		}
	}else{//handled all 'commands', so this must be a calculation or value
		stack_ops(equation);//handle it
	}
  }
  return 0;
}

void stack_ops(char *input){

  if(*input == '+'){
    int result =  pop() + pop();//hold result
    push(result);//put result on stack
  }else if(*input == '-'){
    int v1 = pop();//val 1
    int v2 = pop();//val 2
    push(v2 - v1);//be sure we compute in the right order, add to stack
  }else if(*input == '/'){
    int v1 = pop();//val 1
    int v2 = pop();//val 2
    push(v2/v1);//be sure we compute in the right order, add to stack
  }else if(*input == '*'){
    int result = pop() * pop();//hold result
    push(result);//put result on stack
  }else{//not an operation, but a value.
    push(atoi(input));//add to stack
  }
}

void print_stack(){//print the stack out
  if(isEmptyStack()){
    return;//get out if we are at the end
  }else{
    int temp = pop();//temp to hold value
    printf("%d\n", temp);//display val
    print_stack();//recur
    push(temp);//add value back when we get returned here
  }
}
