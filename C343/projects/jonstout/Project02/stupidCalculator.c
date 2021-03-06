#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

void stackOperations(char *string);
void reverseStack();
void printStack();

int varsOnStack = 0;

int main()
{
  // Basic variables
  char equation[100];

  // Stack setup
  makeEmptyStack();
  printf("Please enter the equation to be evaluated.\n");

  while(1){

    scanf("%s", equation);

    if(*equation == 'q'){
      break;
    }
    else if(*equation == 't'){
      if(!isEmptyStack())
	printf("%d\n", peek());
    }
    else if(*equation == 'p'){
      reverseStack();
      printStack();
    }
    else if(*equation == 'w'){
      while(!isEmptyStack()){
	pop();
      }
    }
    else{
      stackOperations(equation);
    }
  }
  
  return 0;
}

void stackOperations(char *string)
{
  int result;

  if(*string == '+'){
    result =  pop() + pop();
  }
  else if(*string == '-'){
    int second = pop();
    int first = pop();
    push(first - second);
  }
  else if(*string == '*'){
    result = pop() * pop();
    push(result);
  }
  else if(*string == '/'){
    int second = pop();
    int first = pop();
    push(first / second);
  }
  else{
    push( atoi(string) );
    varsOnStack++;
  }
}

void reverseStack()
{

  if(isEmptyStack()){
    return ;
  }
  else{
    int t = pop();
    reverseStack();
    push(t);
  }
}

void printStack()
{
  if(isEmptyStack()){
    return ;
  }
  else{
    int t = pop();
    printf("%d\n", t);
    printStack();
    push(t);
  }
}

