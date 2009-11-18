/* Project 7, C343: Data Structures, Indiana University
 * File: cats.c
 * Name: Greg Patterson, grpatter
 * Date: 11/03/2009
 * Credits:
 * main() concepts: Michael Adams
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "utils.h"
#include "cats.h"

Cat createBaseCat(int i){
	Cat base_cat;
	base_cat.tag = i;
    base_cat.generation = NIL;
    base_cat.sire = NIL;
    base_cat.dam = NIL;
    base_cat.sire_half_sib = NIL;
    base_cat.dam_half_sib = NIL;
    base_cat.eldest_c = NIL;
    base_cat.youngest_c = NIL;
	return base_cat;
}

void init() {
  for(int i = 0; i < MAXCATS; i++) {
    tree[i] = createBaseCat(NIL);
  }
  for(int x = 0; x < DISTSIZE; x++) {
    d_store[x].i = NIL;
    d_store[x].j = NIL;
  }
}

void printCat(Cat cat) {
  printf("Cat: %d\n", cat.tag);
  printf("Cat Generation: %d\n", cat.generation);
  printf("Parents:\tSire: %d\t Dam: %d\n", cat.sire, cat.dam);
  printf("Children:\tEldest: %d\t Youngest: %d \n", cat.eldest_c, cat.youngest_c);
  printf("Siblings:\tSire: %d\t Dam: %d\n", cat.sire_half_sib, cat.dam_half_sib);
}

void printAllCats() {
  for(int i = 0; i < MAXCATS; i++) {
    if(tree[i].tag >= 0) {
		printf("********************\n");
		printCat(tree[i]);
		printf("********************\n");
    } else {
      //no op
    }
  }
}

void printDescs(int cat, int current_gen, bool is_female, int par, int generations) {  
  if(current_gen > 0 && tree[cat].eldest_c != NIL) {
    printf("From cat %d to cat %d is %d generation.\n", cat, par, (generations-current_gen)+1);
    current_gen--;
    is_female = isEven(tree[tree[cat].eldest_c].tag);
    printDescs(tree[cat].eldest_c, current_gen, is_female, par, generations);
    current_gen++;
    if(is_female) {
      if(tree[cat].dam_half_sib != NIL) {
		printDescs(tree[cat].dam_half_sib, current_gen, is_female, par, generations);
      } else {
		//no op
      }
    } else {
      if(tree[cat].sire_half_sib != NIL) {
		printDescs(tree[cat].sire_half_sib, current_gen, is_female, par, generations);
      } else {
		//no op
      }
    }
  } else if (current_gen > 0) {
    printf("From cat %d to cat %d is %d generation.\n", cat, par, (generations-current_gen)+1);
    if(is_female) {
      if(tree[cat].dam_half_sib != NIL) {
		printDescs(tree[cat].dam_half_sib, current_gen, is_female, par, generations);
      } else {
		//no op
      }
    } else {
      if(tree[cat].sire_half_sib != NIL) {
		printDescs(tree[cat].sire_half_sib, current_gen, is_female, par, generations);
      } else {
		//no op
      }
    }
  }
} 

void showDescs(int cat, int generations) {
  bool is_female = isEven(cat);
  if(tree[cat].eldest_c != NIL) {
    printDescs(tree[cat].eldest_c, generations, is_female, cat, generations);
  } else {
    //no op
  }
}

void genTraverse(int c_ind, int cnt, bool reset) { 
  if(reset){
	cnt = NIL;
  }
  if(c_ind != NIL) {
    if(tree[c_ind].generation != NIL) {
      tree[c_ind].generation = minimum(cnt, tree[c_ind].generation);
    } else {
      tree[c_ind].generation = cnt;
    }
    cnt++;
    genTraverse(tree[c_ind].sire, cnt, false);
    genTraverse(tree[c_ind].dam, cnt, false);
  } else {
    return;
  }
}

void printAncestors(int c1, int c2, int d) {
  printf("Cat %d and Cat %d are:\n", c1, c2);
  for(int i = 0; i < DISTSIZE; i++) {
    if(d_store[i].i + d_store[i].j == d) {
      printf("%dth cousins %d times removed.\n", d_store[i].i, d_store[i].j);
    }
  }
}

int getBestDist() {
  int min = NIL;
  for(int i = 0; i < DISTSIZE; i++) {
    if(d_store[i].i + d_store[i].j < min || min == NIL) {
      min = d_store[i].i + d_store[i].j;
    } else {
      //no op
    }
    break;
  }
  return min;
}

void saveDist(int i, int j) {
  for(int a = 0; a < DISTSIZE; a++) {
    if(d_store[a].i == NIL && d_store[a].j == NIL) {      
      d_store[a].i = i;
      d_store[a].j = j;
      break;
    } else {
      //no op
    }
  }
}

void calcDist(int cat, int cnt) {
  if(tree[cat].generation != NIL) {
    saveDist(tree[cat].generation, cnt);
  } else {
    //no op
  }
  if(cat != NIL) {
    cnt++;
    calcDist(tree[cat].sire, cnt);
    calcDist(tree[cat].dam, cnt);
  } else {
	//no op
  }
}

void findRelation(int c_ind1, int c_ind2) {
  tree[c_ind1].generation = 0;
  genTraverse(c_ind1, 0, false);
  calcDist(c_ind2, 0);
  int dist = getBestDist();
  if(dist != NIL) {
    printAncestors(c_ind1, c_ind2, dist);
  } else {
    printf("There is no relation between Cat %d and Cat %d.\n", c_ind1, c_ind2);
  }
  genTraverse(c_ind1, 0, true);
}

void addCat(int c_index, int p_index) {
  Cat p_cat;
  Cat c_cat;
    
  if(tree[p_index].tag == NIL) {//parent
    p_cat = createBaseCat(p_index);
    p_cat.eldest_c = c_index;
    p_cat.youngest_c = c_index;
  } else {
    p_cat = tree[p_index];
    if(p_cat.eldest_c == NIL) {
      p_cat.eldest_c = c_index;
      p_cat.youngest_c = c_index;
    } else {
      if(isEven(p_index)) {
		tree[p_cat.youngest_c].dam_half_sib = c_index;
      } else {
		tree[p_cat.youngest_c].sire_half_sib = c_index;
      }
      p_cat.youngest_c = c_index;
    }
  }
  
  if(tree[c_index].tag == NIL) {//child
    c_cat = createBaseCat(c_index);
  } else {
    c_cat = tree[c_index];
  }
  
  if(isEven(p_index)) {
    if(c_cat.dam == NIL) {
      c_cat.dam = p_index;
    } else {
    }
  } else {
    if(c_cat.sire == NIL) {
      c_cat.sire = p_index;
    } else {
	  //no op
    }
  }
  tree[p_index] = p_cat;//add to tree
  tree[c_index] = c_cat;
}

void cmdHandler(int c_index, int p_index, char *op) {
	if(strcmp(op, "<") == 0) {
		addCat(c_index, p_index);
	} else if(strcmp(op, ">") == 0) {
		addCat(p_index, c_index);
	} else if(strcmp(op, "?") == 0) {
		findRelation(c_index, p_index);
	} else if(strcmp(op, "D") == 0) {
		showDescs(c_index, p_index);
	} else {
		printf("Unrecognized Input: %s\n", op);	
	}
}

int main() {
  char input[101];
  bool done = false;
  int index = 0;
  int vals[2];
  char *op;

  init();//init vals

  while(!done) {                                         
    if (scanf("%100s", input) == EOF) {                                                   
      done = true;
    } else {
		if (isnumber(input)) {
			vals[index++] = atoi(input);
		} else if(strcmp(input, "<") == 0) {
			op = "<";
		} else if(strcmp(input, ">") == 0) {
			op = ">";
		} else if(strcmp(input, "?") == 0) {
			op = "?";
		} else if(strcmp(input, "D") == 0) {
			op = "D";
		} else if(strcmp(input, ".") == 0) {
			cmdHandler(vals[0], vals[1], op);
			index = 0;
			flush_io(); 
		} else if(strcmp(input, "P") == 0) {
			printAllCats(); 
		} else {
			printf("Unrecognized Input: %s\n", input);	
		}
    }
  }
  return 0;
}
 
