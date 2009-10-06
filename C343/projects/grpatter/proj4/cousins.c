// Project 4, C343: Data Structures, Indiana University
// File: cousins.c
// Name: Greg Patterson
// Username: grpatter
// Date: 2009 Sept.

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	int data;
	int parenti;
	bool visited;
}node;

node tree[500];
int cur_i = 0;

bool isnumber(char *str) {
  bool result = true;
  for (int i = 0; str[i] != '\0'; i++) {
    result = result && isdigit(str[i]);
  }
  return result;
}

bool isop(char *str){
	bool result = true;
	if(strcmp(str, ">") == 0 || strcmp(str, "<") == 0 ||strcmp(str, "?") == 0){
		result = true;
	}else{
		result = false;
	}
	return result;
}

void runquery(int val[], char op){
	int match_index = -1;
	int levels = 0;
	int i = val[0];
	while(tree[i].parenti != -1){
		tree[i].visited = true;
		printf("checking tree[%d] : ", i);
		i = tree[i].parenti;
		printf("marked tree visited with parent is %d \n", i);
	}
	int jumps = 0;
	int n = val[1];
	printf("switching to second loop \n");
	while(1){
		if(tree[n].visited){
			printf("tree at %d has been visited already.\n", n);
			break;
		}else{
			jumps++;
			n = tree[n].parenti;
			printf("tree has not been visited, jumping to parent at %d\n", n);
		}
	}
	printf("match found at jumps: %d \n", jumps);
}

void eval(int val[], char op){
	if(op == '<'){//first is child
		node one;
		one.data = val[0];
		one.parenti = val[1];
		node two;
		two.data = val[1];
		two.parenti = -1;
		tree[one.data] = one;
		tree[two.data] = two;
		if(val[0] > cur_i){
			cur_i = val[0];
		}else if(val[1] > cur_i){
			cur_i = val[1];
		}
	}else if(op == '>'){//second is child
		node one;
		one.data = val[0];
		one.parenti = -1;
		node two;
		two.data = val[1];
		two.parenti = val[0];
		tree[one.data] = one;
		tree[two.data] = two;
		if(val[0] > cur_i){
			cur_i = val[0];
		}else if(val[1] > cur_i){
			cur_i = val[1];
		}
	}else if(op == '?'){
		printf("query here!\n");
		runquery(val, op);
	}else{
		printf("Invalid operator found. Ignoring previous statement.\n");
	}
}

int main() {
	init();
	bool done = false;
	char input[101]; // max input is 100 plus 1 for '\0' char
	int val[2];
	int num = 0;
	char op;
	while (!done) {// the "%100s" tells scanf we want no more than 100 chars
		if (scanf("%100s", input) == EOF) {// End of input, assume the user wants to exit
		  done = true;
		}else if(isnumber(input)){
			val[num++] = atoi(input);
		}else if(isop(input)){
			op = input[0];
		}else if(strcmp(input, ".")==0){
			num = 0;
			eval(val, op);
		}else{
			printf("Invalid input.\n");
		}
	}
	return 0;
}

void init(){
	for(int i = 0; i < 500; i++){
		tree[i].parenti = -1;
	}
	reset_visits();
}

void reset_visits(){
	for(int i = 0; i <= 500; i++){
		tree[i].visited = false;
	}
}
