/*
** FILE: familyTree.c
** NAME: Jonathan M. Stout
** DATE: 10/06/09
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Leaf struct
typedef struct{
	int data;
	int parent;
	bool visited;
} Leaf;

// Family Tree
Leaf tree[500];

// Functions
void printParents();
int runquery (int val1, int val2);
int getAnsestor(int val1, int val2);
bool inAncestory(int val, int x);
int calc_dist(int a, int b);
void getRelationship(int i, int j);
void initRelationship(int child, int parent);
bool isnumber(char *str);
void init();

int main() {

    char input[101]; // Input is size 100 plus 1 for '\0'
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
			if(num_arr[0] == num_arr[1]){
				printf("You are yourself. Ignoring.\n");
			}else if (oper == '?') {
				int result = runquery(num_arr[0], num_arr[1]);
				if(result == -1){
					printf("Problem resolving ancestry. Request aborted.\n");
				}
			} else if (oper == '<') {
				if(inAncestory(num_arr[0], num_arr[1]) || inAncestory(num_arr[1], num_arr[0])){
					printf("This would cause a cyclical tree. Ignoring.\n");
				}else{
					initRelationship(num_arr[0], num_arr[1]);
				}
			} else if (oper == '>') {
				if(inAncestory(num_arr[0], num_arr[1]) || inAncestory(num_arr[1], num_arr[0])){
					printf("This would cause a cyclical tree. Ignoring.\n");
				}else{
					initRelationship(num_arr[1], num_arr[0]);
				}
			} else {
				printf("Invalid Operator. Ignoring.\n");
			}
			index = 0;
		} else if(strcmp(input, "pr") == 0){
			printParents();
		}else {
			printf("Invalid input, flushing stdin.\n");
		}
	}
}

// Initilize tree to NULL (basically)
void init() {
	for(int i = 0; i < 500; i++) {
		tree[i].data = -1;
		tree[i].parent = -1;
		tree[i].visited = false;
	}
}

/*
** FUNC: isnumber(char *str)
** SRC : David Wise
*/
bool isnumber(char *str) {
  bool result = true;
  for (int i = 0; str[i] != '\0'; i++) {
    result = result && isdigit(str[i]);
  }
  return result;
}

// Sets a childs parent
void initRelationship(int child, int parent) {
	// If the parent is equal to -1 then set the parent
	if (tree[child].parent == -1)
		tree[child].parent = parent;
	else
		printf("Sorry we don't belive in divorce.\n");
}

// Finds the number of hops to relative
void getRelationship(int i, int j){
	int relationship = abs(j - i);
	printf("%dth cousin %d times removed.`\n", i - 1, relationship);
}

// Distance around the tree
int calc_dist(int a, int b){
	int c = 0;
	int d = a;
	while(d != b){
		d = tree[d].parent;
		c++;
	}
	return c;
}

// Check to see if the child exists in the tree.
bool inAncestory(int val, int x){
	int current = val;
	while(current != -1){
		if(current == x){
			return true;
		}
		else{
			current = tree[current].parent;
		}
	}
return false;
}

// Get the index of the common ansestor
int getAnsestor(int val1, int val2){
int common = -1;
int count1 = 0;
  int count2 = 0;
  if(val2 < val1){
	int tmp = val1;
	val1 = val2;
	val2 = tmp;
  }
  int current = val1;
  while(current != -1) {
	if(count2> 500){
		return -1;
	}
	tree[current].visited = true;
	current = tree[current].parent;
	count1 = count1 + 1;
  }
  current = val2;
  while(current != -1) {
	if(count2 > 500){
		return -1;
	}
	if(tree[current].visited){
		common = current;
		break;
	}else{
		current = tree[current].parent;
		count2 = count2 + 1;
	}
  }
return common;
}

// Runs query
int runquery (int val1, int val2) {
  int com = getAnsestor(val1, val2);
  int di = calc_dist(val1, com);
  int dj = calc_dist(val2, com);
  printf("common:%d di:%d dj:%d\n", com, di, dj);
  if(com == -1){
	printf("No relation.\n");
  	return 0;
  }else{
  	getRelationship(di, dj);
  }  
  return di+dj;
}

// Print Parents
void printParents(){
	for(int i = 0; i < 500; i++){
		if(tree[i].parent != -1){
			printf("tree[%d].parent=%d\n",i,tree[i].parent);
		}
	}
}