/*

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "tree_node.h"

node tree[500];

void init() {	
	for(int i = 0; i < 500; i++) {
		tree[i].data = -1;
		tree[i].parent = -1;
		tree[i].visited = false;
	}
}

bool isnumber(char *str) {//credit: Michael Adams + David Wise
  bool result = true;
  for (int i = 0; str[i] != '\0'; i++) {
    result = result && isdigit(str[i]);
  }
  return result;
}
bool isop(char *str){
	bool result = true;
	if(strcmp(str, ">") == 0 || strcmp(str, "<") == 0 || strcmp(str, "?") == 0){
		result = true;
	}else{
		result = false;
	}
	return result;
}

void create_pair(int child, int par) {
	//check array index at child, if nil set it. otherwise dont
	if(tree[child].parent == -1){
		tree[child].parent = par;//set it
	}else{
		//it exists, dont change
		printf("You can't change your parents.\n");
	}

}

bool common_relation(int i, int j){
	printf("common relation i:%d j:%d\n", i, j);
	if(i == j){
		if(i == 0){ 
			printf("Sibling. \n"); 
			return true; 
		}
	}else if(i < j){
		if(i < 0){
			if(j == 1){//grand
				printf("Great[%d] Grandchild.\n", j-i);
				return true;
			}
		}else if(i == 0){
			printf("Child.\n");
		}else { return false;}
	}else if(i > j){
		return false;
	}else{
		return false;
	}
   return false;
}

void determine_relationship(int i, int j){
	printf("determine_relationship i:%d j:%d\n", i, j);
	int rem = abs(j - i);
		printf("%dth cousin %d times removed.\n", i-1, rem);
}

int calc_dist(int a, int b){
	printf("calcing dist a:%d b:%d\n",a,b);
	int d = 0;
	int x = a;
	while(x != b && x != -1){
		//printf("dist++\n");
		if(x == b){return d;}
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

int run_query (int val1, int val2) {
  printf("run_query 1:%d 2:%d\n", val1, val2);
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

bool eval(int vals[], char op){
	if (op == '?') {
		int result = run_query(vals[0], vals[1]);
		if(result == -1){
			printf("Problem resolving ancestry. Request aborted.\n");
		}
	} else if (op == '<') {
		if(in_ancestry(vals[0], vals[1]) || in_ancestry(vals[1], vals[0])){
			printf("This would cause a cyclical tree. Ignoring.\n");
		}else{
			create_pair(vals[0], vals[1]);
		}
	} else if (op == '>') {
		if(in_ancestry(vals[0], vals[1]) || in_ancestry(vals[1], vals[0])){
			printf("This would cause a cyclical tree. Ignoring.\n");
		}else{
			create_pair(vals[1], vals[0]);
		}
	} else {
		printf("Invalid Operator. Ignoring.\n");
	}
return true;
}

int main() {
    char input[101]; // max input is 100 plus 1 for '\0' char
    bool done = false;
	int vals[2];
	char op;
	int val_n = 0;
	
	init();		
	while(!done) {
		// the "%100s" tells scanf we want no more than 100 chars
		if (scanf("%100s", input) == EOF) {
		    // End of input, assume the user wants to exit
			done = true;
		} else if (isnumber(input)) {
			vals[val_n++] = atoi(input);
		} else if (isop(input)) {
			op = input[0];
		} else if (strcmp(input, ".") == 0) {
			//run data det
			if(vals[0] == vals[1]){
				printf("You are yourself. Ignoring.\n");
			}else{
				eval(vals, op);
			}
			val_n = 0;
		} else if(strcmp(input, "pr") == 0){
			print_parents();
		}else {
			printf("Invalid input, flushing stdin.\n");
			fflushstdin();
		}
	}
}
