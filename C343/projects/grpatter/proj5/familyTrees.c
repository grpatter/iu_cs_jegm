// Project 5, C343: Data Structures, Indiana University
// File: familyTrees.c
// Name: Greg Patterson
// Username: grpatter
// Date: 2009 Oct.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "tree_node.h"
#define NIL -1

node tree[500];

void init() {	//set up tree with values
	for(int i = 0; i < 500; i++) {
		tree[i].data = NIL;
		tree[i].parent = NIL;
		tree[i].visited = false;
		tree[i].eldest_c = NIL;
		tree[i].young_c = NIL;
		tree[i].young_sib = NIL;
		tree[i].children = NIL;
	}
}

bool isnumber(char *str) {//credit: Michael Adams + David Wise
  bool result = true;
  for (int i = 0; str[i] != '\0'; i++) {
    result = result && isdigit(str[i]);
  }
  return result;
}
bool isop(char *str){//check whether given string is acceptable operand
	bool result = true;
	if(strcmp(str, ">") == 0 || strcmp(str, "<") == 0 || strcmp(str, "?") == 0 || strcmp(str, "D") == 0){
		result = true;
	}else{
		result = false;
	}
	return result;
}

void create_pair(int child, int par) {
	//check array index at child, if nil set it. otherwise dont
	if(tree[child].parent == NIL){
		tree[child].parent = par;//set it
		if(tree[par].eldest_c == NIL){//no children, set them
			tree[par].eldest_c = child;
			tree[par].young_c = child;
			tree[par].children++;
		}else{
			int youngc = tree[par].young_c;
			tree[youngc].young_sib = child;
			tree[par].young_c = child;
			tree[par].children++;
		}		
	}else{
		//it exists, dont change
		printf("You can't change your parents.\n");
	}
}

void determine_relationship(int i, int j, bool negate){//print out relationship status
	int rem = abs(j - i);
	int m_i;
		if(j == 0){ m_i = 0;}else{m_i = i-1;}//if same line, we are not ever cousins
			printf("%dth cousin %d times removed. ", m_i, rem);
		if(negate){//negative generation, they are younger
			printf("(They are -%d generations [Younger])\n",rem);
		}else{//positive generation, older or same.
			printf("(They are +%d generations [Older])\n",rem);
		}
}

int calc_dist(int a, int b){//determine distnace in the tree
	int d = 0;
	int x = a;
	while(x != b && x != NIL){
		//printf("dist++\n");
		if(x == b){return d;}
		x = tree[x].parent;
		d++;
	}
	return d;
}

bool in_ancestry(int val, int x){//determine if x is in vals tree
	int cur = val;
	while(cur != NIL){
		if(cur == x){
			return true;
		}else{
			cur = tree[cur].parent;
		}
	}
return false;
}

int find_common(int val1, int val2){//return index of common ancestor
//printf("common on val1:%d val2:%d\n", val1, val2);
int common = -1;
int count1 = 0;

  int count2 = 0;
  int cur = val1;
  while(cur != NIL) {
	if(count1> 500){
		return NIL;
	}
	tree[cur].visited = true;//mark visited
	cur = tree[cur].parent;//adjust to parent link
	count1 = count1 + 1;
  }
  cur = val2;
  while(cur != NIL) {
	if(count2 > 500){
		return NIL;
	}
	if(tree[cur].visited){//we found ancestor
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
  int common = find_common(val1, val2);
  int di = calc_dist(val1, common);
  int dj = calc_dist(val2, common);
  //printf("common node:%d i:%d j:%d\n", common, di, dj);
  if(common == -1){
	printf("No relation.\n");
  	return 0;
  }else{
	printf("%d is %d's ",val1,val2);
		determine_relationship(di, dj, true);
	//if(val1 < val2){//preserve negativity
	//	determine_relationship(dj, di, true);
		//testMe(val1, val2);
	//}else{
	//	determine_relationship(di, dj, false);
		//testMe(val1, val2);
	//}
  }  
  return di+dj;//return total distance found
}

void fflushstdin( void ){//flush anything left
    int c;
    while( (c = fgetc( stdin )) != EOF && c != '\n' );
}

void print_parents(){//print tree in a quick way
	for(int i = 0; i < 500; i++){
		if(tree[i].parent != -1){
			printf("tree[%d].parent=%d\n",i,tree[i].parent);
		}
	}
}

void show_desc(int val1, int val2){
	node temp[500];
	int top = 0;
	printf("Descendents of %d:\n",val1);
	int top_p = val1;
	int cur_p = val1;
	for(int i = 0; i < val2;i++){//loop over given num of gnerations
		printf("Generation[%d]:",i+1);
 		run_query(cur_p, top_p);
			int a = tree[cur_p].young_sib;
			while(a != NIL){//show sibs
				printf("\t");
				run_query(a, top_p);
				a = tree[a].young_sib;
			}
			int x = tree[cur_p].eldest_c;
			//printf("[DEBUG]eldest_c found:%d\n",x);
			while(x != NIL){//show all children
				printf("\t");
				run_query(x, top_p);
				x = tree[x].young_sib;
			}
			cur_p = tree[cur_p].eldest_c;
			printf("\n");
	}
}

bool eval(int vals[], char op){//handle evaluation ops
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
	}else if(op == 'D'){
		show_desc(vals[0],vals[1]);
	} else {
		printf("Invalid Operator. Ignoring.\n");
	}
return true;
}

int main() {
    char input[101];
    bool done = false;
	int vals[2];
	char op;
	int val_n = 0;
	
	init();		//init the tree
	while(!done) {
		if (scanf("%100s", input) == EOF) {
			done = true;
		} else if (isnumber(input)) {//add number to array
			vals[val_n++] = atoi(input);//store for use in execution
		} else if (isop(input)) {
			op = input[0];
		} else if (strcmp(input, ".") == 0) {
			if(vals[0] == vals[1]){
				printf("You are yourself. Ignoring.\n");//me=me!
			}else{
				eval(vals, op);//handle the operation
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
