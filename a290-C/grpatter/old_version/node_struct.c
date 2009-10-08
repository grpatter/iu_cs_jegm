#include <stdlib.h>


struct node {
  char *last_name;
  char *first_name;
  struct node *next_node;
  struct node *previous_node;
};
