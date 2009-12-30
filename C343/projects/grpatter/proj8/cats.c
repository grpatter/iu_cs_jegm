/* File: utils.c
 * Greg Patterson grpatter
 */
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
  int sym_t_entry;
  int counter;
} Cat;

int sym_t[NUM_CATS];
Cat cats[NUM_CATS * 2];

int CATMEM_LOC = 0;
int MEMSIZE = NUM_CATS * 2;
int MEMEND = NUM_CATS;
int spc_ptr = NUM_CATS;

int vals[2];
char *op;
int index;
bool done;
char input[101];


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

  if(cats[cat1].sym_t_entry != -1){
	printf("%d is the ", cats[cat1].sym_t_entry);
  }

  if (count1 == 0 && count2 == 0) {
    printf("same as");
  } else if (count1 == 0) {
    print_great_grand(count2 - 1);
    printf(is_male(cats[cat1].sym_t_entry) ? "father of" : "mother of");
  } else if (count2 == 0) {
    print_great_grand(count1 - 1);
    printf(is_male(cats[cat1].sym_t_entry) ? "son of" : "daughter of");
  } else if (count1 == 1 && count2 == 1) {
    printf(is_male(cats[cat1].sym_t_entry) ? "brother of" : "sister of");
  } else if (count1 == 1) {
    print_great_grand(count2 - 2);
    printf(is_male(cats[cat1].sym_t_entry) ? "uncle of" : "aunt of");
  } else if (count2 == 1) {
    print_great_grand(count1 - 2);
    printf(is_male(cats[cat1].sym_t_entry) ? "nephew of" : "niece of");
  } else {
    int i = min_counter(count1, count2) - 1;
    int j = abs(count1 - count2);
    print_ordinal(i);
    printf(" cousin");
    if (j > 0)
      printf(", %d time%s removed", j, j == 1 ? "" : "s");
    printf(" of");
  }

  if(cats[cat2].sym_t_entry != -1){
	printf(" %d.\n", cats[cat2].sym_t_entry);
  }
}

////////////////
// Functions for '<' and '>'
////////////////

// Gets the next free cell in the cats array
int grabNextCat() {
  int stop;
  if (CATMEM_LOC == 0)
    stop = MEMEND;
  else
    stop = MEMSIZE;

  for (int i = CATMEM_LOC; i < stop; i++) {
    if ( cats[i].sym_t_entry == -1 ) {
      return i;
    }
  }
  return -1;
}

int resolveCat(int i) {
  return sym_t[i];
}

// Adds 'parent' as a sire or dam of 'child'
// after checking for re-parentage and cycles.
void add_parent(CatID child, CatID parent) {

  // Update Child Pointers
  int childPointer = resolveCat(child);
  if (childPointer == NIL) {
    childPointer = grabNextCat();
    cats[childPointer].sym_t_entry = child;
    sym_t[child] = childPointer;
  }

  // check for re-parentage
  if (is_male(parent) ? cats[childPointer].sire != NIL : cats[childPointer].dam != NIL) {
    printf("Input ignored: %d already has a %s.\n",
           child, is_male(parent) ? "sire" : "dam");
    return;
  }

  // Update Parent Pointers
  int parentPointer = resolveCat(parent);
  // printf("%d\n", parentPointer);
  if (parentPointer == NIL) {
    parentPointer = grabNextCat();
    cats[parentPointer].sym_t_entry = parent;
    sym_t[parent] = parentPointer;
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
  //printf("%d %d\n", cat1, cat2);
  cat1 = resolveCat(cat1);
  cat2 = resolveCat(cat2);

  //printf("cat1: %d %d    cat2: %d %d \n", cats[cat1].sym_t_entry, cat1, cats[cat2].sym_t_entry, cat2);

  mark_ancestors(cat1, 0);
  int relationship = find_minimum_relationship(cat2, 0);
  if (relationship == COUNTER_SENTINEL) {
    if(cats[cat1].sym_t_entry == -1 || cats[cat2].sym_t_entry == -1){
		printf("Invalid relationship to non-existant cat.\n");
	}else{
		printf("%d is not related to %d.\n", cats[cat1].sym_t_entry, cats[cat2].sym_t_entry);
	}
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
  cat = resolveCat(cat);

  mark_descendants(cat, max_depth, 0);
  print_descendants(cat, cat, max_depth, 0);
  clear_descendants(cat, max_depth, 0);
}

////////////////
// Save functions
////////////////

int getFirst(int cat, int c) {  
  if (c == 0) {
    return cat;
  } else {
    if (cats[cat].dam == NIL) {
      return cat;
    }else {
      return getFirst(cats[cat].dam, c - 1);
    }
  }
}

void copy(int i, int j) {
  cats[i].eldest_child = cats[j].eldest_child;
  cats[i].next_younger_by_sire = cats[j].next_younger_by_sire;
  cats[i].next_younger_by_dam = cats[j].next_younger_by_dam; 
  cats[i].sym_t_entry = cats[j].sym_t_entry;
  sym_t[cats[j].sym_t_entry] = i;
  //printf("Copied cat from %d to %d...\n",i,j);
}

void setupLinks() {
  for(int i = CATMEM_LOC; i < MEMEND; i++) {
    CatID cur = cats[i].eldest_child;
	while(cur != NIL) {
		cats[i].youngest_child = cur;
		if(is_male(i)){
			cats[cur].sire = i;
			cur = cats[cur].next_younger_by_sire;
		}else{
			cats[cur].dam = i;
			cur = cats[cur].next_younger_by_dam;
		}
		//printf("links loop\n");
	}
  }
}

int moveCat(int c) {
  if (cats[c].eldest_child != NIL) {    
    cats[c].eldest_child = moveCat(cats[c].eldest_child);
  }
  if (cats[c].next_younger_by_sire != NIL) {
    cats[c].next_younger_by_sire = moveCat(cats[c].next_younger_by_sire);
  }
  if (cats[c].next_younger_by_dam != NIL) {
    cats[c].next_younger_by_dam = moveCat(cats[c].next_younger_by_dam);
  }

  copy(spc_ptr, c);
  return spc_ptr++;
}

int moveDam(int i, int j) {
  cats[i].eldest_child = cats[j].eldest_child;
  cats[i].next_younger_by_sire = -1;
  cats[i].next_younger_by_dam = -1; 
  cats[i].sym_t_entry = cats[j].sym_t_entry;
  sym_t[cats[j].sym_t_entry] = i;
  return spc_ptr++;
}

void clearDestSpace(int st, int end) {
  for (int i = st; i < end; i++) {
    cats[i].sire = NIL;
    cats[i].dam = NIL;
    cats[i].eldest_child = NIL;
    cats[i].youngest_child = NIL;
    cats[i].next_younger_by_sire = NIL;
    cats[i].next_younger_by_dam = NIL;
    cats[i].sym_t_entry = -1;
    cats[i].counter = COUNTER_SENTINEL;    
  }
}

void saveDescs(int cat, int c) {
	cat = resolveCat(cat);
	int first_cat = getFirst(cat, c);
	int cur_eld = -1;
	
	if (cats[first_cat].eldest_child != NIL) {
		cur_eld = moveCat(cats[first_cat].eldest_child);
	}
	first_cat = moveDam(spc_ptr, first_cat);
	cats[first_cat].eldest_child = cur_eld;
	
	if (is_male(first_cat)){
		cats[cur_eld].sire = first_cat;
	}else{
		cats[cur_eld].dam = first_cat;
	}
	
	if (CATMEM_LOC == 0) {
		CATMEM_LOC = NUM_CATS;
		MEMEND = NUM_CATS * 2;
		spc_ptr = 0;
	}else {
		CATMEM_LOC = 0;
		MEMEND = NUM_CATS;
		spc_ptr = NUM_CATS;
	}

	setupLinks();
}

void saveHandler(char input[101]) {
	int cat;
	int l;
	bool is_cat = true;
	char *str;

	str = strtok(input, "@");
	while (str != NULL) {
		if (is_cat) {
			cat = atoi(str);
			is_cat = false;
		} else {
			l = atoi(str);
		}
		str = strtok(NULL, "@");
	}
	saveDescs(cat, l);
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

void cmdHandler(int c_index, int p_index, char *op) {
	if(strcmp(op, "<") == 0) {
		add_parent(c_index, p_index);
	} else if(strcmp(op, ">") == 0) {
		add_parent(p_index, c_index);
	} else if(strcmp(op, "?") == 0) {
		query_relationship(c_index, p_index);
	} else if(strcmp(op, "D") == 0) {
		query_descendants(c_index, p_index);
	} else if (strcmp(op, "Save") == 0) {
      if (CATMEM_LOC == 0) {
		clearDestSpace(NUM_CATS, NUM_CATS*2); 
      }else {
		clearDestSpace(0, NUM_CATS); 
      }
    }else {
		printf("Unrecognized Input: %s\n", op);	
	}
}

int main() {
  for (unsigned int i = 0; i < sizeof(cats) / sizeof(cats[0]); i++) {
    cats[i].sire = NIL;
    cats[i].dam = NIL;
    cats[i].eldest_child = NIL;
    cats[i].youngest_child = NIL;
    cats[i].next_younger_by_sire = NIL;
    cats[i].next_younger_by_dam = NIL;
    cats[i].sym_t_entry = -1;
    cats[i].counter = COUNTER_SENTINEL;
  }
  for (int i = 0; i < NUM_CATS; i++) {
    sym_t[i] = -1;
  }
  cats[-1].sire = NIL;
  cats[-1].dam = NIL;

  while(!done) {
    if (scanf("%100s", input) == EOF) {
      // End of input, assume the user wants to exit
      done = true;
    }
	else {
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
			op = "Save";
		}else if (strcmp(input, ".") == 0) {
			cmdHandler(vals[0], vals[1], op);
			index = 0;
		}else { 
			if (strcmp(op, "Save") == 0) {
				saveHandler(input);
				printf("Save operation + cleanup completed.\n");
			} else {
				printf("Unrecognized Input: %s\n", op);	
			}
		}
	}
  }

  return 0;
}
