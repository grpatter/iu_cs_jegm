#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

void stackOperations(char *string);

int main()
{
  // Basic variables
  char equation[100];

  // Stack setup
  makeEmptyStack();
  printf("Please enter the equation to be evaluated.\n");

  while(1){

    scanf("%s", &equation);

    if(*equation == 'q'){
      break;
    }
    else if(*equation == 't'){
      printf("%d\n", peek());
    }
    else if(*equation == 'p'){
      printStack();
    }
    else if(*equation == 'w'){
      wipeStack();
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
    push(result);
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
  }
}
