/*
** FILE: linkedCalculator.c
** NAME: Jonthan M. Stout
** DATE: 09/29/2009
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "stack.h"
#include <math.h>
#include "AVAIL.h"

void stackOperations(char *string);
void printStack();
QueueData figureIdenAndAnni(QueueData first, QueueData second, char oper);

int main()
{
  // Basic variables
  char equation[100];

  // Stack Init
  makeEmptyStack();
  printf("Please enter the equation to be evaluated.\n");

  while(1){
    
    // Get first char of input and store into equation
    scanf("%s", equation);

    // Check for commands <quit>, <peek>, <print stack>, <wipe stack>.
    if( strcmp(equation, "q") == 0){
      break;
    }
    else if(strcmp(equation, "t") == 0){ 
      if(!isEmptyStack()) {
	QueueData temp = peek();
	if(temp.tag == 0)
	  printf("%d\n", temp.myUnion.i);
	else
	  printf("%c\n", temp.myUnion.c);
      }
    }
    else if(strcmp(equation, "p") == 0){
      printStack();
    }
    else if(strcmp(equation, "w") == 0){
      while(!isEmptyStack()){
	pop();
      }
    }
    else if(strcmp(equation, "X") == 0){
      QueueData tempUnion;
      tempUnion.tag = 2;
      tempUnion.myUnion.c = 'X';
      push(tempUnion);
    }
    else if(strcmp(equation, "Y") == 0){
      QueueData tempUnion;
      tempUnion.tag = 2;
      tempUnion.myUnion.c = 'Y';
      push(tempUnion);
    }
    else if(strcmp(equation, "Z") == 0){
      QueueData tempUnion;
      tempUnion.tag = 2;
      tempUnion.myUnion.c = 'Z';
      push(tempUnion);
    }
    else{
      // Delegate to stackOperations(char *string)
      stackOperations(equation);
    }
  }
  
  return 0;
}

// Parse all numbers and algebreic signs
void stackOperations(char *string)
{
  QueueData result;

  //Addition
  if(strcmp(string, "+") == 0){
    QueueData second = pop();
    QueueData first = pop();
    
    if(first.tag == 0 && second.tag == 0) {
      result.tag = 0;
      result.myUnion.i = first.myUnion.i + second.myUnion.i;
      push(result);
    }
    else if (first.tag == 2 || second.tag == 2) {
      push(figureIdenAndAnni(first, second, '+'));
    }
    else {
      push(first);
      push(second);
    }
  }
  //Subtration
  else if(strcmp(string, "-") == 0){
    QueueData second = pop();
    QueueData first = pop();

    if(first.tag == 0 && second.tag == 0) {
      result.tag = 0;
      result.myUnion.i = first.myUnion.i - second.myUnion.i;
      push(result);
    }
    else if (first.tag == 2 || second.tag == 2) {
      push(figureIdenAndAnni(first, second, '-'));
    }
    else {
      push(first);
      push(second);
    }
  }
  // Multiplication
  else if(strcmp(string, "*") == 0){
    QueueData second = pop();
    QueueData first = pop();

    if(first.tag == 0 && second.tag == 0) {
      result.tag = 0;
      result.myUnion.i = first.myUnion.i * second.myUnion.i;
      push(result);
    }
    else if (first.tag == 2 || second.tag == 2) {
      push(figureIdenAndAnni(first, second, '*'));
    }
    else {
      push(first);
      push(second);
    }
  }
  // Division
  else if(strcmp(string, "/") == 0){
    QueueData second = pop();
    QueueData first = pop();

    if(first.tag == 0 && second.tag == 0) {
      result.tag = 0;
      result.myUnion.i = first.myUnion.i / second.myUnion.i;
      push(result);
    }
    else if (first.tag == 2 || second.tag == 2) {
      push(figureIdenAndAnni(first, second, '/'));
    }
    else {
      push(first);
      push(second);
    }
  }
  // Exponential
  else if(strcmp(string, "^") == 0){
    QueueData second = pop();
    QueueData first = pop();

    if(first.tag == 0 && second.tag == 0) {
      result.tag = 0;
      result.myUnion.i = pow(first.myUnion.i, second.myUnion.i);
      push(result);
    }
    else if (first.tag == 2 || second.tag == 2) {
      push(figureIdenAndAnni(first, second, '^'));
    }
    else {
      push(first);
      push(second);
    }
  }
  // If number store on stack as an int
  else{
    QueueData tempUnion;
    tempUnion.tag = 0;
    tempUnion.myUnion.i = atoi(string);
    push(tempUnion);
  }
}

// Print Stack
void printStack()
{
  if(isEmptyStack()){
    return ;
  }
  else{
    QueueData t = pop();
    if(t.tag == 0) 
      printf("%d\n", t.myUnion.i);
    if(t.tag == 2 || t.tag == 1)
      printf("%c\n", t.myUnion.c);
    printStack();
    push(t);
  }
}

// Identities and Annihilators
QueueData figureIdenAndAnni(QueueData first, QueueData second, char oper)
{
  if (first.tag == 0) {
    if (first.myUnion.i == 0 && oper == '+') {
      return second;
    }
    else if (first.myUnion.i == 0 && oper == '*') {
      return first;
    }
    else if (first.myUnion.i == 0 && oper == '/') {
      return first;
    }
    else {
      return second;
    }
  }
  else {
    if (second.myUnion.i == 0 && oper == '+' || second.myUnion.i == 0 && oper == '-' ||
	second.myUnion.i == 1 && oper == '*' || second.myUnion.i == 1 && oper == '/') {
      return first;
    }
    else if (oper == '*') {
      return second;
    }
    else {
      QueueData temp;
      temp.tag = 0;
      temp.myUnion.i = 1;
      return temp;
    }
  }
}
