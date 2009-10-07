/*Greg Patterson (grpatter)
A290 Assignment Link List Project
*/

/*
For this list, you should support at least the following operations:

add_val();-Add a value to the list.
ret_element_n();-Return the nth element in the list.
ret_first();-Return the first element in the list.
ret_last();-Return the last element in the list.
rem_element();-Remove an arbitrary element in the list.
search_ls();-Search for an arbitrary element by either first or last name.
rev_ls();-Reverse the list.
sort_ls();-Sort the list by either first or last name, in either ascending or descending order.
print_ls();-Print the list.
ret_len();-Return the length of the list.
*/

#include <stdio.h>
#include <stdlib.h>

#define LAST 1 /*nodes last name*/
#define FIRST 0 /*nodes first name*/
#define ASCEND 1 /*goes from A-Z*/
#define DESCEND 0 /*goes from Z-A*/

struct node *add_node(struct node *head, char *first, char *last, int name_order, int sort_order);/*prototype*/
struct node *add_node_handle(struct node *head);/*prototype*/
void print_list(struct node *head);/*prototype*/

struct node *add_node(struct node *head, char *first, char *last, int name_order, int sort_order) {
  
  struct node *temp_node = NULL;  
  
  temp_node = (struct node *) malloc(sizeof(struct node));
  temp_node->next_node = NULL;
  temp_node->previous_node = NULL;

  temp_node->first_name = (char *) malloc(strlen(first) + 1);
  strcpy(temp_node->first_name, first);
  temp_node->last_name = (char *) malloc(strlen(last) + 1);
  strcpy(temp_node->last_name, last);
  
  if (head == NULL) {
    head = temp_node;
  } else {
    struct node *cursor = head;
    if (name_order == LAST) { 
      if (sort_order == ASCEND) {
		if (strcmp(cursor->last_name, temp_node->last_name) > 0) {
		  cursor->next_node = temp_node;
		  temp_node->previous_node = cursor;
		  head = temp_node;
		} else {
		  while(strcmp(cursor->last_name,temp_node->last_name) < 0) {
			if(cursor->previous_node != NULL) {
			  cursor = cursor->previous_node;
			} else {
			  break;
			}
		  }
		  temp_node->previous_node = cursor->previous_node;
		  temp_node->next_node = head;
		  if(temp_node->previous_node != NULL) {
			temp_node->previous_node->next_node = temp_node;
		  }
		  cursor->previous_node = temp_node;
		}
      } else if (sort_order == DESCEND) {
			if (strcmp(cursor->last_name, temp_node->last_name) < 0) {
			  cursor->next_node = temp_node;
			  temp_node->previous_node = cursor;
			  head = temp_node;
			} else {
			  while(strcmp(cursor->last_name,temp_node->last_name) > 0) {
				if(cursor->previous_node != NULL) {
				  cursor = cursor->previous_node;
				} else {
				  break;
				}
			  }
			  temp_node->previous_node = cursor->previous_node;
			  temp_node->next_node = head;
			  if(temp_node->previous_node != NULL) {
				temp_node->previous_node->next_node = temp_node;
			  }
			  cursor->previous_node = temp_node;
			}	  
        } else {
			printf("you entered an incorrect value for sorting, please try again");
		}      
    } else if (name_order == FIRST) {
      if (sort_order == ASCEND) {
		if (strcmp(cursor->first_name, temp_node->first_name) > 0) {
		  cursor->next_node = temp_node;
		  temp_node->previous_node = cursor;
		  head = temp_node;
		} else {
		  while(strcmp(cursor->first_name,temp_node->first_name) > 0) {
			if(cursor->previous_node != NULL) {
			  cursor = cursor->previous_node;
			} else {
			  break;
			}
		  }
		  temp_node->previous_node = cursor->previous_node;
		  temp_node->next_node = head;
		  if(temp_node->previous_node != NULL) {
			temp_node->previous_node->next_node = temp_node;
		  }
		  cursor->previous_node = temp_node;
		}
      } else if (sort_order == DESCEND) {
		if (strcmp(cursor->first_name, temp_node->first_name) < 0) {
		  cursor->next_node = temp_node;
		  temp_node->previous_node = cursor;
		  head = temp_node;
		} else {
		  while(strcmp(cursor->first_name,temp_node->first_name) < 0) {
			if(cursor->previous_node != NULL) {
			  cursor = cursor->previous_node;
			} else {
			  break;
			}
		  }
		  temp_node->previous_node = cursor->previous_node;
		  temp_node->next_node = head;
		  if(temp_node->previous_node != NULL) {
			temp_node->previous_node->next_node = temp_node;
		  }
		  cursor->previous_node = temp_node; 
		}
      }  
    } else {
      printf("you entered an incorrect value for sorting, please try again");
    }
  }
  return head;
}

struct node *add_node_handle(struct node *head){	
	char first_nm[24];
	char last_nm[24];
	int name_o = 0;
	int dir_o = 1;
	
	printf("You have chosen to add a node to the list.\nPlease specify the first name: ");
	scanf("%23s",&first_nm);
	printf("Please specify the last name: ");
	scanf("%23s",&last_nm);
	printf("\nIf you wish to insert by first name, enter 0. If you wish to insert by last name, enter 1: ");
	scanf("%d",&name_o);		
	printf("\nIf you wish to insert in descending (Z>A) order, enter 0. If you wish to insert in ascending (A>Z) order, enter 1: ");
	scanf("%d",&dir_o);
	return add_node(head, first_nm, last_nm, name_o, dir_o);
}

struct node *rem_element(struct node *head){
	int type;
	int list_index;
	int list_length = ret_len(head);
	printf("Please make a choice below.\n");
	printf("1) Remove by first name.\n");
	printf("2) Remove by last name.\n");
	printf("3) Remove by location.\n");
	scanf("%d",&type);
	if(type == 1){
		//f_name
		char f_name[24];
		printf("\nPlease enter the first name: ");
		scanf("%23s",&f_name);
	}else if(type == 2){
		//l_name
		char last_name[24];
		printf("\nPlease enter the last name: ");
		scanf("%23s",&last_name);
	}else{
		//location
		int loc, list_index;
		int list_length = ret_len(head);
		struct node* cursor = head;
		printf("\nPlease enter the location: ");
		scanf("%d",&loc);
		  if(head == NULL) {
			return head;
		  } else {
			if(list_length >= loc) {
			  for(list_index = 1; list_index <= list_length; list_index++) {
				if(list_index == loc) {
					if(loc == 1){
						head = head->next_node;
					}
					if(cursor->next_node != NULL){
						cursor->next_node->previous_node = cursor->previous_node;
					}
					if(cursor->previous_node != NULL){
						cursor->previous_node->next_node = cursor->next_node;
					}
					cursor->next_node = NULL;
					cursor->previous_node = NULL;
					free(cursor->last_name);
					free(cursor->first_name);
					free(cursor);
					return head;
				} else {
				  cursor = cursor->previous_node;
				}
			  }
			} else {
			  printf("\nThe location you requested could not be removed.");
			  return head;
			}
		  }
	}
}
void print_list(struct node *head) {
  if(head == NULL) {
	return;
  } else {
    while(head != NULL) {
      printf("First Name: %s \tLast Name: %s\n", head->first_name, head->last_name);
	  if(head->previous_node != NULL){
		head = head->previous_node;
	  }else{
		break;
	  }
    }
  }
}

