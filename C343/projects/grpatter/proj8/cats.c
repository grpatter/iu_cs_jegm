/*
** NAME: Jonathan M. Stout
** FILE: cats.c
** SRC : (c) 2009, Michael D. Adams <adamsmd>
*/

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEBUG 0
#define NIL ((CatID)-1)
#define COUNTER_SENTINEL (-1)

////////////////
// Types
////////////////

typedef int CatID;
typedef struct {
  CatID sire;
  CatID dam;
  CatID eldest_child;
  CatID youngest_child;
  CatID next_younger_by_sire;
  CatID next_younger_by_dam;
  int tableEntry;
  int counter;
} Cat;

Cat cats[NUM_CATS * 2];
int symTable[NUM_CATS];

int num_arr[2];
char oper;
int index;
bool done;
char input[101];
int catMemory = 0;
int memSize = NUM_CATS * 2;
int memoryEnd = NUM_CATS;

////////////////
// Helper functions
////////////////



// Returns true iff 'cat' is male.
bool is_male(CatID cat) { return cat % 2 != 0; }

// Returns the minimum of 'x' and 'y'.
// COUNTER_SENTINEL is treated as positive infinity.
int min_counter(int x, int y) {
  return
    x == COUNTER_SENTINEL ? y :
    y == COUNTER_SENTINEL ? x :
    x < y ? x : y;
}

// Returns the sum of 'x' and 'y'.
// COUNTER_SENTINEL is treated as positive infinity.
int add_counter(int x, int y) {
  return
    x == COUNTER_SENTINEL ? COUNTER_SENTINEL :
    y == COUNTER_SENTINEL ? COUNTER_SENTINEL :
    x + y;
}

// Returns true iff 'ancestor' is an ancestor of 'cat'
bool is_ancestor(CatID cat, CatID ancestor) {
  if (cat == NIL) return false;
  else if (cat == ancestor) return true;
  else return is_ancestor(cats[cat].sire, ancestor) ||
              is_ancestor(cats[cat].dam, ancestor);
}

// Returns the sire sibling of 'cat' iff 'is_male' is true.
// Otherwise returns the dam sibling of 'cat'.
CatID get_sibling(CatID cat, bool is_male) {
  if (is_male) { return cats[cat].next_younger_by_sire; }
  else { return cats[cat].next_younger_by_dam; }
}

// Prints the number of 'greats' and 'grands' specified by 'i'.
void print_great_grand(int i) {
  for (; i > 1; i--)
    printf("great-");
  if (i > 0)
    printf("grand");
}

// Prints 'i' as an ordinal number (e.g. 1st, 2nd, 3rd, etc.).
void print_ordinal(int i) {
  if (10 <= (i % 100) && (i % 100) <= 13) printf("%dth", i);

  switch (i % 10) {
  case 1: printf("%dst", i); break;
  case 2: printf("%dnd", i); break;
  case 3: printf("%drd", i); break;
  default: printf("%dth", i); break;
  }
}

// Prints "<cat1> is the <relationship> of <cat2>.\n"
void print_relationship(CatID cat1, int count1, CatID cat2, int count2) {
  assert(count1 != COUNTER_SENTINEL);
  assert(count2 != COUNTER_SENTINEL);

  printf("%d is the ", cats[cat1].tableEntry);

  if (count1 == 0 && count2 == 0) {
    printf("same as");
  } else if (count1 == 0) {
    print_great_grand(count2 - 1);
    printf(is_male(cat1) ? "father of" : "mother of");
  } else if (count2 == 0) {
    print_great_grand(count1 - 1);
    printf(is_male(cat1) ? "son of" : "daughter of");
  } else if (count1 == 1 && count2 == 1) {
    printf(is_male(cat1) ? "brother of" : "sister of");
  } else if (count1 == 1) {
    print_great_grand(count2 - 2);
    printf(is_male(cat1) ? "uncle of" : "aunt of");
  } else if (count2 == 1) {
    print_great_grand(count1 - 2);
    printf(is_male(cat1) ? "nephew of" : "niece of");
  } else {
    int i = min_counter(count1, count2) - 1;
    int j = abs(count1 - count2);
    print_ordinal(i);
    printf(" cousin");
    if (j > 0)
      printf(", %d time%s removed", j, j == 1 ? "" : "s");
    printf(" of");
  }

  printf(" %d.\n", cats[cat2].tableEntry);
}

////////////////
// Functions for '<' and '>'
////////////////

// Gets the next free cell in the cats array
int nextFreeCat() {
  int end;
  if (catMemory == 0)
    end = memoryEnd;
  else
    end = memSize;

  for (int i = catMemory; i < end; i++) {
    if ( cats[i].tableEntry == -1 ) {
      return i;
    }
  }
  return -1;
}

int getCat(int index) {
  return symTable[index];
}

// Adds 'parent' as a sire or dam of 'child'
// after checking for re-parentage and cycles.
void add_parent(CatID child, CatID parent) {
  int childPointer = getCat(child);
  // printf("%d ", childPointer);
  if (childPointer == NIL) {
    childPointer = nextFreeCat();
    cats[childPointer].tableEntry = child;
    symTable[child] = childPointer;
  }

  int parentPointer = getCat(parent);
  // printf("%d\n", parentPointer);
  if (parentPointer == NIL) {
    parentPointer = nextFreeCat();
    cats[parentPointer].tableEntry = parent;
    symTable[parent] = parentPointer;
  }

  // check for re-parentage
  if (is_male(parent) ? cats[childPointer].sire != NIL : cats[childPointer].dam != NIL) {
    printf("Input ignored: %d already has a %s.\n",
           child, is_male(parent) ? "sire" : "dam");
    return;
  }

  // check for cycles
  if (is_ancestor(parent, child)) {
    printf("Input ignored: it would cause cycle\n");
    return;
  }

  // set parent pointer
  if (is_male(parent)) cats[childPointer].sire = parentPointer;
  else cats[childPointer].dam = parentPointer;

  // set child pointer
  if (cats[parentPointer].eldest_child == NIL) {
    // this is first child to be added
    assert(cats[parentPointer].youngest_child == NIL);
    cats[parentPointer].eldest_child = childPointer;
    cats[parentPointer].youngest_child = childPointer;
  } else {
    // this is not the first child to be added
    CatID youngest_child = cats[parentPointer].youngest_child;
    assert(youngest_child != NIL);
    if (is_male(parent)) {
      assert(cats[youngest_child].next_younger_by_sire == NIL);
      cats[youngest_child].next_younger_by_sire = childPointer;
    } else {
      assert(cats[youngest_child].next_younger_by_dam == NIL);
      cats[youngest_child].next_younger_by_dam = childPointer;
    }
    cats[parentPointer].youngest_child = childPointer;
  }
}

////////////////
// Functions for '?'
////////////////

// Sets the 'counter' on all ancestors of 'cat' based on 'counter'.
void mark_ancestors(CatID cat, int counter) {
  if (cat == NIL) return;
  else {
    cats[cat].counter = min_counter(counter, cats[cat].counter);
    mark_ancestors(cats[cat].sire, counter + 1);
    mark_ancestors(cats[cat].dam, counter + 1);
  }
}

// Returns the smallest 'cats[cat].counter+counter' on all ancestors of 'cat'.
int find_minimum_relationship(CatID cat, int counter) {
  if (cat == NIL) return COUNTER_SENTINEL;
  else {
    return
      min_counter(
        add_counter(counter, cats[cat].counter),
        min_counter(
          find_minimum_relationship(cats[cat].sire, counter + 1),
          find_minimum_relationship(cats[cat].dam, counter + 1)));
  }
}

// Prints all relationships between 'cat1' and 'cat2' of the given 'distance'.
void print_relationship_at_distance(
  CatID cat1, CatID cat2, CatID ancestor, int distance, int counter) {
  if (ancestor == NIL) return;
  else {
    // print self iff at 'distance'
    if (distance == add_counter(counter, cats[ancestor].counter)) {
      print_relationship(cat1, cats[ancestor].counter, cat2, counter);
    }

    // recur on sire
    print_relationship_at_distance(
      cat1, cat2, cats[ancestor].sire, distance, counter + 1);

    // recur on dam
    print_relationship_at_distance(
      cat1, cat2, cats[ancestor].dam, distance, counter + 1);
  }
}

// Sets the 'counter' on all ancestors of 'cat' to 'COUNTER_SENTINEL'.
void clear_ancestors(CatID cat) {
  if (cat == NIL) return;
  else {
    cats[cat].counter = COUNTER_SENTINEL;
    clear_ancestors(cats[cat].sire);
    clear_ancestors(cats[cat].dam);
  }
}

// Prints all closest relationships between 'cat1' and 'cat2'.
void query_relationship(CatID cat1, CatID cat2) {
  cat1 = getCat(cat1);
  cat2 = getCat(cat2);

  mark_ancestors(cat1, 0);
  int relationship = find_minimum_relationship(cat2, 0);
  if (relationship == COUNTER_SENTINEL) {
    printf("%d is not related to %d.\n", cats[cat1].tableEntry, cats[cat2].tableEntry);
  } else {
    print_relationship_at_distance(cat1, cat2, cat2, relationship, 0);
  }
  clear_ancestors(cat1);
}

////////////////
// Functions for 'D'
////////////////

// Sets the 'counter' on the descendants of 'cat'.
void mark_descendants(CatID cat, int max_depth, int curr_depth) {
  if (cat == NIL) return;
  else if (curr_depth > max_depth) return;
  else {
    // mark self
    cats[cat].counter = min_counter(curr_depth, cats[cat].counter);

    // recur on children by looping though the eldest's siblings
    for (CatID child = cats[cat].eldest_child;
         child != NIL; child = get_sibling(child, is_male(cat))) {
      mark_descendants(child, max_depth, curr_depth + 1);
    }
  }
}

// Prints the descendants of 'curr'.
void print_descendants(CatID root, CatID curr, int max_depth, int curr_depth) {
  if (curr == NIL) return;
  else if (curr_depth > max_depth) return;
  else {
    // print self
    if (curr_depth == cats[curr].counter) {
      printf("At generation %d, ", curr_depth);
      print_relationship(curr, curr_depth, root, 0);

      // ensure we don't print this cat again
      cats[curr].counter = COUNTER_SENTINEL;
    }

    // recur on children by looping though the eldest's siblings
    for (CatID child = cats[curr].eldest_child;
         child != NIL; child = get_sibling(child, is_male(curr))) {
      print_descendants(root, child, max_depth, curr_depth + 1);
    }
  }
}

// Sets the 'counter' on all the descendants of 'cat' to 'COUNTER_SENTINEL'.
void clear_descendants(CatID cat, int max_depth, int curr_depth) {
  if (cat == NIL) return;
  else if (curr_depth > max_depth) return;
  else {
    // clear self
    cats[cat].counter = COUNTER_SENTINEL;

    // recur on children by looping though the eldest's siblings
    for (CatID child = cats[cat].eldest_child;
         child != NIL; child = get_sibling(child, is_male(cat))) {
      clear_descendants(child, max_depth, curr_depth + 1);
    }
  }
}

// Prints the descendants of 'cat' down to 'max_depth'.
void query_descendants(CatID cat, int max_depth) {
  cat = getCat(cat);

  mark_descendants(cat, max_depth, 0);
  print_descendants(cat, cat, max_depth, 0);
  clear_descendants(cat, max_depth, 0);
}

////////////////
// Save function
////////////////

void save_descendants() {
  
}

void mark_saved_cats(int cat, int lvl) {
  printf("%d cat, %d lvl(s)\n", cat, lvl);

  
}

void parse_saved_cats(char input[101]) {
  int cat;
  int lvl;
  bool isCat = true;
  char *temp;

  temp = strtok(input, "@");
  while (temp != NULL) {
      if (isCat) {
      cat = atoi(temp);
      isCat = false;
    }
    else {
      lvl = atoi(temp);
    }
    temp = strtok(NULL, "@");
  }
  mark_saved_cats(cat, lvl);
}

////////////////
// Main function
////////////////

typedef enum {
  OPERATOR_ERROR = -1,
  LESS_THAN,
  GREATER_THAN,
  QUESTION,
  DESCENDANTS
} Operator;

Operator parse_operator(char str[]) {
  if (strcmp(str, "<") == 0) return LESS_THAN;
  if (strcmp(str, ">") == 0) return GREATER_THAN;
  if (strcmp(str, "?") == 0) return QUESTION;
  if (strcmp(str, "D") == 0) return DESCENDANTS;

  printf("ERROR: unknown operator '%s'\n", str);
  return OPERATOR_ERROR;
}

// Begin ----------------------------------------------------
void handleInput(char input[101]) {
  if (isnumber(input)) {
    num_arr[index++] = atoi(input);
  }
  else if (strcmp(input, "?") == 0 || strcmp(input, "<") == 0 || strcmp(input, ">") == 0 || strcmp(input, "D") == 0 || strcmp(input, "Save") == 0) {
    oper = input[0];
  }
  else if (strcmp(input, ".") == 0) {
    if (oper == '?') {
      query_relationship(num_arr[0], num_arr[1]);
    } 
    else if (oper == 'D') {
      query_descendants(num_arr[0], num_arr[1]);
      printf("\n");
    }
    else if (oper == '<') {
      add_parent(num_arr[0], num_arr[1]);
    } 
    else if (oper == '>') {
      add_parent(num_arr[1], num_arr[0]);
    }
    else if (oper == 'S') {
      save_descendants();
    }
    else {
      printf("Invalid Operator. Ignoring.\n");
    }
    index = 0;
  }
  else { 
    if (oper == 'S') {
      parse_saved_cats(input);
    }
    else {
      printf("Invalid input, flushing stdin.\n");
    }
  }
}
// End ------------------------------------------------------

void collectorStart(int a1, int a2, int b1, int b2, int c1, int c2){
//call descs and mark each node at given roots (a1, b1, c1)
//query_descendants_mark(a1,a2);
//query_descendants_mark(b1,b2);
//query_descendants_mark(c1,c2);
//run Cheney scan loop over tree and copy to to_space
	//copy_to_toSpace();
//adjust pointers/forwarding as necessary
//free from_space
	//clear_descendants(a1, a2, 0);
	//clear_descendants(b1, b2, 0);
	//clear_descendants(c1, c2, 0);
	/*
	for(int i = 0; i < MAXCATS;i++){
		free(cats[i]);
	}
	*/
//reset globals
}

void cmdHandler(int c_index, int p_index, char *op) {
	if(strcmp(op, "<") == 0) {
		add_parent(c_index, p_index);
	} else if(strcmp(op, ">") == 0) {
		add_parent(p_index, c_index);
	} else if(strcmp(op, "?") == 0) {
		query_relationship(c_index, p_index);
	} else if(strcmp(op, "D") == 0) {
		query_descendants(c_index, p_index);
	} else {
		printf("Unrecognized Input: %s\n", op);	
	}
}

void saveHandler(char *arr[]){
	printf("Save requested with roots:");
	for(int i = 0; i < 3; i++){
		printf("%s\t",arr[i]);
	}
	int s1a = atoi(&arr[0][0]);
	int s1b = atoi(&arr[0][2]);
	int s2a = atoi(&arr[1][0]);
	int s2b = atoi(&arr[1][2]);
	int s3a = atoi(&arr[2][0]);
	int s3b = atoi(&arr[2][2]);
	printf("\n");
}

int main() {
  // initialize the cats  
  for (unsigned int i = 0; i < sizeof(cats) / sizeof(cats[0]); i++) {
    cats[i].sire = NIL;
    cats[i].dam = NIL;
    cats[i].eldest_child = NIL;
    cats[i].youngest_child = NIL;
    cats[i].next_younger_by_sire = NIL;
    cats[i].next_younger_by_dam = NIL;
    cats[i].tableEntry = -1;
    cats[i].counter = COUNTER_SENTINEL;
  }
  cats[-1].sire = NIL;
  cats[-1].dam = NIL;

  for (int i = 0; i < NUM_CATS; i++) {
    symTable[i] = -1;
  }
  
  int sym_ind = 0;
  char input[101];
  bool done = false;
  int index = 0;
  int vals[2];
  char save_1[4];
  char save_2[4];
  char save_3[4];
  int saveindex = 0;
  char *op;
  bool save_action = false;

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
		} else if(strcmp(input, "Save") == 0){
			save_action = true;
			
			scanf("%4s", save_1);
			//printf("we should store: %s\n", save_1);
			
			scanf("%4s", save_2);
			//printf("we should store: %s\n", save_2);
			
			scanf("%4s", save_3);
			//printf("we should store: %s\n", save_3);
			
		} else if(strcmp(input, ".") == 0) {
			if(save_action){
				char *save_arr[] = {save_1, save_2, save_3};
				saveHandler(save_arr);
			}else{
				cmdHandler(vals[0], vals[1], op);
			}
			index = 0;
			saveindex = 0;
			save_action = false;
		} else if(strcmp(input, "P") == 0) {
			//printAllCats(); 
		} else {
			printf("Unrecognized Input: %s\n", input);	
		}
    }
  }

  return 0;
}
