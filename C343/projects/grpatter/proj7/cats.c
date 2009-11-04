
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define NIL -1
//#define DEBUG 1
#define DEBUG 0


typedef struct {
  int info;
  int gen_count;
  int sire_par;
  int dam_par;
  int sire_next_halfSib;
  int dam_next_halfSib;
  int eldest_child;
  int youngest_child;
} Feline;

typedef struct {
  int i;
  int j;
} dist;

Feline tree[500];
dist dist_list[50];

void flush_io() {
  fflush(stdin);
  fflush(stdout);
}

enum Cats {SIRE, DAM};
enum Input {NUM, GREAT, LESS, ANS, DESC, PER, PRINT};

void init() {
  if(DEBUG) printf("Initializing Felines\n");
  for(int i = 0; i < 500; i++) {
    tree[i].info = NIL;
    tree[i].gen_count = NIL;
    tree[i].sire_par = NIL;
    tree[i].dam_par = NIL;
    tree[i].sire_next_halfSib = NIL;
    tree[i].dam_next_halfSib = NIL;
    tree[i].eldest_child = NIL;
    tree[i].youngest_child = NIL;
  }
  for(int x = 0; x < 50; x++) {
    dist_list[x].i = NIL;
    dist_list[x].j = NIL;
  }
}



//_______________________________Parsing___________________________________
//source David Wise                                                                                      
bool isnumber(char *str) {
  bool result = true;
  for (int i = 0; str[i] != '\0'; i++) {
    result = result && isdigit(str[i]);
  }
  return result;
}
//_______________________________Parsing___________________________________


int input_cmp(char *input) {
  if (isnumber(input)) {
    if(DEBUG) printf("input_cmp: %d\n", atoi(input));
    return NUM;
  } else if(strcmp(input, "<") == 0) {
    if(DEBUG) printf("input_cmp: %s\n", input);
    return GREAT;
  } else if(strcmp(input, ">") == 0) {
    if(DEBUG) printf("input_cmp: %s\n", input);
    return LESS;
  } else if(strcmp(input, "?") == 0) {
    if(DEBUG) printf("input_cmp: %s\n", input);
    return ANS;
  } else if(strcmp(input, "D") == 0) {
    if(DEBUG) printf("input_cmp: %s\n", input);
    return DESC;
  } else if(strcmp(input, ".") == 0) {
    if(DEBUG) printf("input_cmp: %s\n", input);
    return PER; 
  } else if(strcmp(input, "P") == 0) {
    if(DEBUG) printf("input_cmp: %s\n", input);
    return PRINT; 
  } else {
    if(DEBUG) printf("incorrect input: %s\n", input);
    return NIL;
  }
}

//_____________________________Printing Section____________________________________
void printFel(Feline cat) {
  printf("Feline: %d ", cat.info);
  printf("Feline level: %d", cat.gen_count);
  printf("Sire Parent: %d, Dam Parent: %d ", cat.sire_par, cat.dam_par);
  printf("Eldest Child: %d, Youngest Child: %d \n\n", cat.eldest_child, cat.youngest_child);
  printf("Next Sire Sibling: %d, Next Dam Sibling: %d ", cat.sire_next_halfSib, cat.dam_next_halfSib);
}

void printArr(int num_arr[]) {
  printf("Input[0]: %d, Input[1]: %d\n", num_arr[0], num_arr[1]);
}

void printFel_array() {
  for(int i = 0; i < 500; i++) {
    if(tree[i].info >= 0) {
      printf("___________________________Feline: %d _________________\n", tree[i].info);
      printf("Sire Parent: %d, Dam Parent: %d \n", tree[i].sire_par, tree[i].dam_par);
      printf("Level count: %d\n", tree[i].gen_count);
      printf("Next Sire Sibling: %d, Next Dam Sibling: %d \n", tree[i].sire_next_halfSib, tree[i].dam_next_halfSib);
      printf("Eldest Child: %d, Youngest Child: %d \n", tree[i].eldest_child, tree[i].youngest_child);
    } else {
      //Feline not created at this location
    }
  }
}

void print_distList() {
  for(int i = 0; i < 50; i++) {
    if(dist_list[i].i != NIL && dist_list[i].j != NIL) {
      printf("distance is %d with i: %d and j: %d\n", dist_list[i].i + dist_list[i].j, dist_list[i].i, dist_list[i].j); 
    }
  }
}

//_____________________________Printing Section____________________________________

bool isEven(int num) {
  bool even = false;
  if(num % 2 == 0) {
    even = true;
  } else {
    even = false;
  }
  return even;
}

int minum(int num1, int num2) {
  if(num1 < num2) {
    return num1;
  } else {
    return num2;
  }
}

//_______________________________________________descendants______________________
//3 < 1 . 1 < 11 . 1 < 6 . 2 < 5 . 5 < 11 . 5 < 10 . 2 < 4 . 4 < 9 .
//1 ? 2 .
// 11 D 2 .


void list_desc(int cat, int gen, bool gender, int par, int start_gen) {
  
  if(gen > 0 && tree[cat].eldest_child != NIL) {
    //print that cat and move to its eldest child
    printf("Feline %d is %d generation(s) from feline %d\n", cat, (start_gen-gen)+1, par);
    gen--;
    gender = isEven(tree[tree[cat].eldest_child].info);
    list_desc(tree[cat].eldest_child, gen, gender, par, start_gen);
    gen++;
    if(gender) {
      if(tree[cat].dam_next_halfSib == NIL) {
	//nothing there return
	if(DEBUG) printf("Feline: %d does not have dam next half sibling\n", cat);
      } else {
	list_desc(tree[cat].dam_next_halfSib, gen, gender, par, start_gen);
      }
    } else {
      if(tree[cat].sire_next_halfSib == NIL) {
	//nothing there return
	if(DEBUG) printf("Feline: %d does not have sire next half sibling\n", cat);
      } else {
	list_desc(tree[cat].sire_next_halfSib, gen, gender, par, start_gen);
      }
    } 
  } else if (gen > 0) {
    //print the siblings
    printf("Feline %d is %d generation(s) from feline %d\n", cat, (start_gen-gen)+1, par);
    if(gender) {
      if(tree[cat].dam_next_halfSib == NIL) {
	//nothing there return
	if(DEBUG) printf("Feline: %d does not have dam next half sibling\n", cat);
      } else {
	list_desc(tree[cat].dam_next_halfSib, gen, gender, par, start_gen);
      }
    } else {
      if(tree[cat].sire_next_halfSib == NIL) {
	//nothing there return
	if(DEBUG) printf("Feline: %d does not have sire next half sibling\n", cat);
      } else {
	list_desc(tree[cat].sire_next_halfSib, gen, gender, par, start_gen);
      }
    } 
  }
}     

void start_desc(int cat, int gen) {
  bool gender = isEven(cat);
  if(DEBUG) printf("____________________start listing descendants______________\n");
  if(tree[cat].eldest_child != NIL) {
    list_desc(tree[cat].eldest_child, gen, gender, cat, gen);
  } else {
    //is leaf doesn't have any descendants
  }
}

void set_level(int cat, int counter, bool init_count) {
  if(DEBUG) printf("Setting level: %d for cat: %d\n", counter, cat);  
  if(init_count){
	counter = NIL;
  }
  if(cat == NIL) {
    return;
  } else {
    if(tree[cat].gen_count != NIL) {
      tree[cat].gen_count = minum(counter, tree[cat].gen_count);
    } else {
      //it doesn't have a count already just set it
      tree[cat].gen_count = counter;
    }
    counter++;
    set_level(tree[cat].sire_par, counter, false);
    set_level(tree[cat].dam_par, counter, false);
  }
}

//_____________________________________________ancestry____________________
//_______________________________________________descendants______________________

void printAnc(int dist) {
  for(int i = 0; i < 50; i++) {
    if(dist_list[i].i + dist_list[i].j == dist) {
      printf("%dth cousin, %d times removed\n", dist_list[i].i, dist_list[i].j);
    }
  }
}

int distanceArray() {
  int min_dist = NIL;
  for(int i = 0; i < 50; i++) {    
    if(DEBUG) printf("dist_list[%d]: %d \n", i, min_dist);
    if(dist_list[i].i + dist_list[i].j < min_dist || min_dist == NIL) {
      if(DEBUG) printf("inside dist_list[%d]: %d \n", i, min_dist);
      min_dist = dist_list[i].i + dist_list[i].j;
      if(DEBUG) printf("after calc dist_list[%d]: %d \n", i, min_dist);
    } else {
      //ignore not smaller
    }
    return min_dist;
  }
  return min_dist;
}

void store_distance(int i, int j) {
  if(DEBUG) printf("i: %d, j: %d\n", i, j);
  
  for(int y = 0; y < 50; y++) {
    if(dist_list[y].i == NIL && dist_list[y].j == NIL) {      
      dist_list[y].i = i;
      dist_list[y].j = j;
      break;
    } else {
      //don't put anything in there you are at an index that has something
    }
  }
}

void find_distance(int cat, int counter) {
  if(DEBUG) printf("cat %d generation count: %d\n", cat, tree[cat].gen_count);
  if(tree[cat].gen_count != NIL) {
    if(DEBUG) printf("Feline: %d level count: %d\n", cat, tree[cat].gen_count);
    store_distance(tree[cat].gen_count, counter);
  } else {
    //not a common ancestor
  }
  if(cat == NIL) {
    //return nothing here
  } else {
    counter++;
    find_distance(tree[cat].sire_par, counter);
    find_distance(tree[cat].dam_par, counter);
  }
}

void det_ancs(int cat1, int cat2) {
  if(DEBUG) printf("Determing Relationship between %d, %d\n", cat1, cat2);
  tree[cat1].gen_count = 0;
  set_level(cat1, 0, false);
  find_distance(cat2, 0);
  int dist = distanceArray();
  if (DEBUG) printf("dist: %d\n", dist);
  if(dist != NIL) {
    printf("Feline %d and Feline %d are:\n", cat1, cat2);
    printAnc(dist);
  } else {
    printf("Feline %d and Feline %d are not related\n", cat1, cat2);
  }
  if(DEBUG) print_distList();
  if(DEBUG) printf("Shortest distance between cat: %d and cat: %d is %d\n", cat1, cat2, dist);
  set_level(cat1, 0, true);//reset values to nil
}
//_____________________________________________descendants____________________

///_____________________________________________input____________________

void input_cats(int child, int par) {
  if(DEBUG) printf("parent: %d, child: %d\n", par, child);
  Feline child_cat;
  Feline par_cat;
  
  if(tree[child].info == NIL) {//Child doesn't exist
    child_cat.info = child;
    child_cat.gen_count = NIL;
    child_cat.sire_par = NIL;
    child_cat.dam_par = NIL;
    child_cat.sire_next_halfSib = NIL;
    child_cat.dam_next_halfSib = NIL;
    child_cat.eldest_child = NIL;
    child_cat.youngest_child = NIL;
  } else {//Child exist
    child_cat = tree[child];
  }

  if(tree[par].info == NIL) {//Parent doesn't exist
    par_cat.info = par;
    par_cat.gen_count = NIL;
    par_cat.sire_par = NIL;
    par_cat.dam_par = NIL;
    par_cat.sire_next_halfSib = NIL;
    par_cat.dam_next_halfSib = NIL;
    par_cat.eldest_child = child;
    par_cat.youngest_child = child;
  } else {//Parent exist
    par_cat = tree[par];
    if(par_cat.eldest_child == NIL) {
      par_cat.eldest_child = child;
      par_cat.youngest_child = child;
    } else {
      if(isEven(par)) {
	tree[par_cat.youngest_child].dam_next_halfSib = child;
      } else {
	tree[par_cat.youngest_child].sire_next_halfSib = child;
      }
      par_cat.youngest_child = child;
    }
  }
  
  if(isEven(par)) {
    if(child_cat.dam_par == NIL) {
      child_cat.dam_par = par;
    } else {
      if(DEBUG) printf("child: %d already has dam parent: %d\n", child, child_cat.dam_par);
    }
  } else {
    if(child_cat.sire_par == NIL) {
      child_cat.sire_par = par;
    } else {
      if(DEBUG) printf("child: %d already has sire parent: %d\n", child, child_cat.sire_par);
    }
  }
  tree[child] = child_cat;
  tree[par] = par_cat;
}

void run_instruct(int child, int par, int oper) {
  switch (oper) {
  case GREAT : input_cats(child, par); if(DEBUG) printf("running greater than\n"); break;
  case LESS : input_cats(par, child);  if(DEBUG) printf("running less than\n"); break;
  case ANS : det_ancs(child, par); break;
  case DESC : start_desc(child, par); break;
  default : if(DEBUG) printf("Incorrect Operator\n"); break;
  }
  
}

int main() {
  char input[101]; //max input is 100 plus 1 for '\0' char
  bool done = false;
  int index = 0;
  int input_arr[2];
  char *oper;

  //----------------Testing printFel-------------------
  if(DEBUG) printf("______________Feline Test_________________\n");
  Feline testCat;
  testCat.info = 1;
  testCat.sire_par = 5;
  testCat.dam_par = 7;
  testCat.sire_next_halfSib = 2;
  testCat.dam_next_halfSib = 4;
  testCat.eldest_child = 8;
  testCat.youngest_child = 9;
  if (DEBUG) printFel(testCat);
  if(DEBUG) printf("______________Feline Test_________________\n");
  //----------------Testing printFel-------------------
  if(DEBUG) printf("______________isEven Test_________________\n");  
  int even_num = 6;
  int odd_num = 7;
  if(DEBUG) if(isEven(6)) printf("even_num: %d\n", even_num);
  if(DEBUG) if(!isEven(7)) printf("odd_num: %d\n", odd_num);
  if(DEBUG) printf("______________Feline Test_________________\n");
  //________________Testing isEven_____________________

  init();//initialize Feline values

  while(!done) {
    // the "%100s" tells scanf we want no more than 100 chars                                           
    if (scanf("%100s", input) == EOF) {
      // End of input, assume the user wants to exit                                                     
      done = true;
    } else {
      int case_num = input_cmp(input);
      if(DEBUG) printf("input comparison: %d\n", case_num);
      switch (case_num) {
      case NUM : input_arr[index++] = atoi(input); if(DEBUG) printArr(input_arr); break;
      case GREAT : oper = "<"; break;
      case LESS : oper = ">"; break;
      case ANS : oper = "?"; break; 
      case DESC : oper = "D"; break;
      case PER : run_instruct(input_arr[0], input_arr[1], input_cmp(oper)); index = 0; flush_io(); break;
      case PRINT : printFel_array(); break; 
      default : printf("input incorrect: %s\n", input);
      }
    }
  }
  return 0;
}
 
