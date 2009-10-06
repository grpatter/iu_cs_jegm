/*

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct{
	int data;
	int parent;
	bool visited;
} node;

node tree[500];

void init() {
	
	for(int i = 0; i < 500; i++) {
		tree[i].data = -1;
		tree[i].parent = -1;
		tree[i].visited = false;
	}
}

bool isnumber(char *str) {
  bool result = true;
  for (int i = 0; str[i] != '\0'; i++) {
    result = result && isdigit(str[i]);
  }
  return result;
}

void input_rel(int par, int child) {
	//check and create parent
	if (tree[par].data != -1) {
		node parent;
		parent.data = par;
		tree[par] = parent;
	} else {
		//parent already exist don't do anything
	}
	
	//check and create child
	if (tree[child].data != -1) {
		node kid;
		kid.data = child;
		kid.parent = par;
		tree[child] = kid;
	} else {
		//if it already exist just set the parent
		tree[child].parent = par;
	}
}

int runquery (int val1, int val2) {
  int counter = 0;
  printf("val1: %d, val2: %d \n", val1, val2);
  while(tree[val1].parent != -1) {
	printf("parent: %d\n", tree[val1].parent);
	tree[val1].visited = true;
	val1 = tree[val1].parent;
  }
  
  while(tree[val2].parent != -1 || tree[val2].visited == true) {
	val2 = tree[val2].parent;
	counter++;
  }
  
  return counter;
}

int main() {

    char input[101]; // max input is 100 plus 1 for '\0' char
    bool done = false;
	int num_arr[2];
	char oper;
	int index = 0;
	
	init();
		
	while(!done) {
		// the "%100s" tells scanf we want no more than 100 chars
		if (scanf("%100s", input) == EOF) {
		    // End of input, assume the user wants to exit
			done = true;
		} else if (isnumber(input)) {
			num_arr[index++] = atoi(input);
		} else if (strcmp(input, "?") == 0 || strcmp(input, "<") == 0 || strcmp(input, ">") == 0) {
			oper = input[0];
		} else if (strcmp(input, ".") == 0) {
			//run data det
			printf("%c\n", oper);
			if (oper == '?') {
				int result = runquery(num_arr[0], num_arr[1]);
				printf("%d\n", result);
			} else if (oper == '<') {
				printf("child: %d parent: %d\n", num_arr[0], num_arr[1]);
				input_rel(num_arr[0], num_arr[1]);
			} else if (oper == '>') {
				input_rel(num_arr[1], num_arr[0]);
			} else {
				printf("Incorrect operator\n");
			}
			index = 0;
			printf("num1: %d num2: %d oper: %c\n", num_arr[0], num_arr[1], oper);
		} else {
			printf("incorrect input\n");
		}
	}
}
