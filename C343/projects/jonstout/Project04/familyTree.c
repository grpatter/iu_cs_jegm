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

Leaf tree[500];

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

void input_rel(int child, int par) {
	//check array index at child, if nil set it. otherwise dont
	if(tree[child].parent == -1){
		tree[child].parent = par;//set it
	}else{
		//it exists, dont change
		printf("You can't change your parents.\n");
	}

}

void determine_relationship(int i, int j){
	int rem = abs(j - i);
	printf("%dth cousin %d times removed.`\n", i-1, rem);
}

int calc_dist(int a, int b){
	int d = 0;
	int x = a;
	while(x != b){
		x = tree[x].parent;
		d++;
	}
	return d;
}

bool in_ancestry(int val, int x){
	int cur = val;
	while(cur != -1){
		if(cur == x){
			return true;
		}else{
			cur = tree[cur].parent;
		}
	}
return false;
}

int find_common(int val1, int val2){
int common = -1;
int count1 = 0;
  int count2 = 0;
  if(val2 < val1){
	int tmp = val1;
	val1 = val2;
	val2 = tmp;
  }
  int cur = val1;
  while(cur != -1) {
	if(count2> 500){
		return -1;
	}
	tree[cur].visited = true;
	cur = tree[cur].parent;
	count1 = count1 + 1;
  }
  cur = val2;
  while(cur != -1) {
	if(count2 > 500){
		return -1;
	}
	if(tree[cur].visited){
		common = cur;
		break;
	}else{
		cur = tree[cur].parent;
		count2 = count2 + 1;
	}
  }
return common;
}

int runquery (int val1, int val2) {
  int common = find_common(val1, val2);
  int di = calc_dist(val1, common);
  int dj = calc_dist(val2, common);
  printf("common:%d di:%d dj:%d\n", common, di, dj);
  if(common == -1){
	printf("No relation.\n");
  	return 0;
  }else{
  	determine_relationship(di, dj);
  }  
  return di+dj;
}


void fflushstdin( void ){
    int c;
    while( (c = fgetc( stdin )) != EOF && c != '\n' );
}


void print_parents(){
	for(int i = 0; i < 500; i++){
		if(tree[i].parent != -1){
			printf("tree[%d].parent=%d\n",i,tree[i].parent);
		}
	}
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
			if(num_arr[0] == num_arr[1]){
				printf("You are yourself. Ignoring.\n");
			}else if (oper == '?') {
				int result = runquery(num_arr[0], num_arr[1]);
				if(result == -1){
					printf("Problem resolving ancestry. Request aborted.\n");
				}
			} else if (oper == '<') {
				if(in_ancestry(num_arr[0], num_arr[1]) || in_ancestry(num_arr[1], num_arr[0])){
					printf("This would cause a cyclical tree. Ignoring.\n");
				}else{
					input_rel(num_arr[0], num_arr[1]);
				}
			} else if (oper == '>') {
				if(in_ancestry(num_arr[0], num_arr[1]) || in_ancestry(num_arr[1], num_arr[0])){
					printf("This would cause a cyclical tree. Ignoring.\n");
				}else{
					input_rel(num_arr[1], num_arr[0]);
				}
			} else {
				printf("Invalid Operator. Ignoring.\n");
			}
			index = 0;
		} else if(strcmp(input, "pr") == 0){
			print_parents();
		}else {
			printf("Invalid input, flushing stdin.\n");
			fflushstdin();
		}
	}
}
