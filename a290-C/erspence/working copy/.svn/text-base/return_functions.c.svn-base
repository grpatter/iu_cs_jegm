/* Eric Spencer (erspence)
   A290 Unix and C, Spring 2009
   Linked-List project (return functions)
   February 13th (Friday)
*/

#include <stdio.h>
#include <stdlib.h>

#define LAST 1 /*nodes last name*/
#define FIRST 0 /*nodes first name*/
#define ASCEND 1 /*goes from A-Z*/
#define DESCEND 0 /*goes from Z-A*/
struct node *insert_node(struct node *head, char *first, char *last, int name_order, int sort_order);
struct node *search_list(struct node* head, char *first, char *last, int name_order);
struct node *get_nth_node(struct node *head, int location);
struct node *get_head_node(struct node *head);
struct node *remove_node(struct node *head, char *first, char *last, int name_order);
struct node *get_tail_node(struct node *head);
int get_list_length(struct node *head);
struct node *reverse_list(struct node *head);
struct node *sort_list(struct node *head, int name_order, int sort_order);

struct node *insert_node(struct node *head, char *first, char *last, int name_order, int sort_order) {
  
  struct node *temp_node = NULL;
  
  /*create node for the list */
  temp_node = (struct node *) malloc(sizeof(struct node));

  /*set node's pointers to NULL */ 
  temp_node->next_node = NULL;
  temp_node->previous_node = NULL;

  /*set first and last name of node*/
  temp_node->first_name = (char *) malloc(strlen(first) + 1);
  strcpy(temp_node->first_name, first);
  temp_node->last_name = (char *) malloc(strlen(last) + 1);
  strcpy(temp_node->last_name, last);

  if (head == NULL) {
    head = temp_node;
    return head;
  } else {
    struct node *cursor = head;
    if (name_order == LAST) { /*last name*/
      /* you will be checking to insert by last name */
      if (sort_order == ASCEND) {/*ascend*/
	/*iterate through the list */
	while(cursor != NULL) {
	  if(strcmp(temp_node->last_name, cursor->last_name) < 0) {
	    if(cursor->next_node != NULL) {
	      cursor->next_node->previous_node = temp_node;
	    }
	    temp_node->previous_node = cursor;
	    temp_node->next_node = cursor->next_node;
	    cursor->next_node = temp_node;
	    return get_head_node(head);
	  } else if (cursor->previous_node == NULL) {
	    cursor->previous_node = temp_node;
	    temp_node->next_node = cursor;
	    return get_head_node(head);
	  } else {
	    cursor = cursor->previous_node;
	  }
	}
      } else if (sort_order == DESCEND) {/*descend*/
	/*iterate through the list */
	while(cursor != NULL) {
	  if(strcmp(temp_node->last_name, cursor->last_name) > 0) {
	    if(cursor->next_node != NULL) {
	      cursor->next_node->previous_node = temp_node;
	    }
	    temp_node->previous_node = cursor;
	    temp_node->next_node = cursor->next_node;
	    cursor->next_node = temp_node;  
	    return get_head_node(head);
	  } else if (cursor->previous_node == NULL) {
	    cursor->previous_node = temp_node;
	    temp_node->next_node = cursor;
	    return get_head_node(head);
	  } else {
	    cursor = cursor->previous_node;
	  }
	}
      } else {
	printf("error in sorting by last name\n");
      }
    } else if (name_order == FIRST) {/*first name*/
      /* you will be checking to insert by first name */
      if (sort_order == ASCEND) {/*ascend*/
	/*iterate through the list */
	while(cursor != NULL) {
	  if(strcmp(temp_node->first_name, cursor->first_name) < 0) {
	    if(cursor->next_node != NULL) {
	      cursor->next_node->previous_node = temp_node;
	    }
	    temp_node->previous_node = cursor;
	    temp_node->next_node = cursor->next_node;
	    cursor->next_node = temp_node;   
	    return get_head_node(head);
	  } else if (cursor->previous_node == NULL) {
	    cursor->previous_node = temp_node;
	    temp_node->next_node = cursor;
	    return get_head_node(head);
	  } else {
	    cursor = cursor->previous_node;
	  }
	}
      } else if (sort_order == DESCEND) {/*descend*/
	/*iterate through the list */
	while(cursor != NULL) {
	  if(strcmp(temp_node->first_name, cursor->first_name) > 0) {
	    if(cursor->next_node != NULL) {
	      cursor->next_node->previous_node = temp_node;
	    }
	    temp_node->previous_node = cursor;
	    temp_node->next_node = cursor->next_node;
	    cursor->next_node = temp_node;  
	    return get_head_node(head);
	  } else if (cursor->previous_node == NULL) {
	    cursor->previous_node = temp_node;
	    temp_node->next_node = cursor;
	    return get_head_node(head);
	  } else {
	    cursor = cursor->previous_node;
	  }
	}  
      } else {
	printf("there was an error in printing first name\n");
      }
    } 
  }
}

struct node *search_list(struct node* head, char *first, char *last, int name_order) {
  struct node *cursor = head;
  
  if(cursor == NULL) {
    return;
  } else {
    if(name_order == LAST) {
      printf("I make it into the last name section of search_list\n");
      /*will need to search through the list to see if there is a match*/
      while(cursor != NULL) {
	if(strcmp(cursor->last_name, last) == 0) {
	  return cursor;
	} else {
	  cursor = cursor->previous_node;
	}
      }
      return NULL;
    } else if (name_order == FIRST) {
      /*will need to search through the list to see if there is a match*/
      
      while(cursor != NULL) {
	if(strcmp(cursor->first_name, first) == 0) {
	  return cursor;
	} else {
	  cursor = cursor->previous_node;
	}
      }
      return NULL;
    } else {
      printf("you have enetered an incorrect value for name_order\n");
    }
  }
}


/*return the nth node as a struct node*/
struct node *get_nth_node(struct node *head, int location) {
  int list_index;
  int list_length = get_list_length(head);
  struct node* cursor = head;
  if(head == NULL) {
    return head;
  } else {
    if(list_length > location) {
      for(list_index = 1; list_index < list_length; list_index++) {
	if(list_index == location) {
	  return cursor;
	} else {
	  cursor = cursor->previous_node;
	}
      }
    } else {
      printf("You have requested a position that does exist in this linked-list");
    }
  }
}

/*return a pointer to the head node, to get information or something*/
struct node *get_head_node(struct node *head) {
  struct node *cursor = head;
  if (cursor == NULL) {
    printf("your list is empty\n");
  } else {
    while(cursor->next_node != NULL) {
      cursor = cursor->next_node;
    }
  } 
  return cursor;
}


struct node *remove_node(struct node *head, char *first, char *last, int name_order) {
  struct node *dead_node = search_list(head, first, last, name_order);
  struct node *new_head = NULL;
  if(dead_node == NULL) {
    return NULL;
  } else {
    if(dead_node == get_head_node(head)) {
      new_head = dead_node->previous_node;
    }
    if(dead_node->next_node != NULL) {
      dead_node->next_node->previous_node = dead_node->previous_node;
    }
    if(dead_node->previous_node != NULL) {
      dead_node->previous_node->next_node = dead_node->next_node;
    }
    dead_node->next_node = NULL;
    dead_node->previous_node = NULL;
    free(dead_node->last_name);
    free(dead_node->first_name);
    free(dead_node);
    return new_head;
  }
}

struct node *get_tail_node(struct node *head) {
  if(head == NULL) {
    return head;
  } else {
    while(head->previous_node != NULL) {
      head = head->previous_node;
    }
    return head;
  }
}

int get_list_length(struct node *head) {
  int node_length = 0;
  
  if(head == NULL) {
    return node_length;
  } else {
    while(head != NULL) {
      node_length++;
      head = head->previous_node;
    }  
  }
  return node_length;
}

struct node *reverse_list(struct node *head) {
  if (head == NULL) return NULL;
  struct node *cursor = get_head_node(head);
  
  while (cursor->previous_node != NULL) {
    struct node *temp = cursor->previous_node;
    cursor->previous_node = cursor->next_node;
    cursor->next_node = temp;
    cursor = temp; /*cursor->next_node;*/
  }
  
  struct node *temp_node = cursor->previous_node;
  cursor->previous_node = cursor->next_node;
  cursor->next_node = temp_node;

  return cursor;
}

struct node *sort_list(struct node *head, int name_order, int sort_order) {
  if(head == NULL) return head;
  struct node *temp = NULL;
  while(head != NULL) {
    temp = insert_node(temp, head->first_name, head->last_name, name_order, sort_order);
    head = remove_node(head, head->first_name, head->last_name, 1);
    if(head == NULL) {
      //temp = insert_node(temp, head->first_name, head->last_name, name_order, sort_order);
      break;
    }
  }
  return get_head_node(temp);
}
