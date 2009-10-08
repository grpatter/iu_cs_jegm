/* Greg Patterson (grpatter)
	A290
*/

#include <stdio.h>
#include <stdlib.h>

#define LAST 1 /*last name*/
#define FIRST 0 /*first name*/
#define ASCEND 1 /*for A-Z order*/
#define DESCEND 0 /*for Z-A order*/
/*START prototypes*/
struct node *add_node(struct node *head, char *first, char *last, int name_order, int sort_order);
struct node *search_ls(struct node* head, char *first, char *last, int name_order);
struct node *get_nnode(struct node *head, int location);
struct node *get_head(struct node *head);
struct node *remove_node(struct node *head, char *first, char *last, int name_order);
struct node *get_tail(struct node *head);
int get_ls_len(struct node *head);
struct node *reverse_ls(struct node *head);
struct node *sort_ls(struct node *head, int name_order, int sort_order);
/*END prototypes*/

/*add a value to the list and return pointer to the updated list */
struct node *add_node(struct node *head, char *first, char *last, int name_order, int sort_order) {
	struct node *temp_node = NULL;

	/*create temp node to store data until we place it */
	temp_node = (struct node *) malloc(sizeof(struct node));

	/*set up node initial information */ 
	temp_node->next = NULL;
	temp_node->prev = NULL;
	temp_node->first_nm = (char *) malloc(strlen(first) + 1);
	strcpy(temp_node->first_nm, first);/*insert string after making space */
	temp_node->last_nm = (char *) malloc(strlen(last) + 1);
	strcpy(temp_node->last_nm, last);/*insert string after making space */

	if (head == NULL) {
		head = temp_node;
		return head;/*return parameters as a node if no list exists */
	} else {
	struct node *cursor = head;/*use this for iterating */
	if (name_order == LAST) { /*insert by last name*/
		if (sort_order == ASCEND) {/*insert in ascending order*/
			while(cursor != NULL) {/*make sure we can make changes */
				if(strcmp(temp_node->last_nm, cursor->last_nm) < 0) {
					if(cursor->next != NULL) {
						cursor->next->prev = temp_node;
					}
					temp_node->prev = cursor;
					temp_node->next = cursor->next;
					cursor->next = temp_node;/*set cursor to the built temp node*/
					return get_head(head);/*update ptrs */
				} else if (cursor->prev == NULL) {
					cursor->prev = temp_node;/*update ptrs */
					temp_node->next = cursor;/*update ptrs */
					return get_head(head);
				} else {
					cursor = cursor->prev;/*update ptrs */
				}
			}
		} else if (sort_order == DESCEND) {/*insert in descending order*/
		/*process is same as for LAST/ASCEND inserting but with differnt string comparison */
			while(cursor != NULL) {
				if(strcmp(temp_node->last_nm, cursor->last_nm) > 0) {
					if(cursor->next != NULL) {
						cursor->next->prev = temp_node;
					}
					temp_node->prev = cursor;
					temp_node->next = cursor->next;
					cursor->next = temp_node;  
					return get_head(head);
				} else if (cursor->prev == NULL) {
					cursor->prev = temp_node;
					temp_node->next = cursor;
				return get_head(head);
				} else {
					cursor = cursor->prev;
				}
			}
		} else {
			printf("Could not sort by parameters given. Request failed.\n");
		}
	} else if (name_order == FIRST) {/*insert by last name*/
		if (sort_order == ASCEND) {/*insert in ascending order*/
		/*process is same as for LAST/ASCEND inserting but with differnt string comparison */
			while(cursor != NULL) {
				if(strcmp(temp_node->first_nm, cursor->first_nm) < 0) {
					if(cursor->next != NULL) {
						cursor->next->prev = temp_node;
					}
					temp_node->prev = cursor;
					temp_node->next = cursor->next;
					cursor->next = temp_node;   
					return get_head(head);
				} else if (cursor->prev == NULL) {
					cursor->prev = temp_node;
					temp_node->next = cursor;
					return get_head(head);
				} else {
					cursor = cursor->prev;
				}
			}
		} else if (sort_order == DESCEND) {/*insert in descending order*/
		/*process is same as for LAST/ASCEND inserting but with differnt string comparison */
			while(cursor != NULL) {
				if(strcmp(temp_node->first_nm, cursor->first_nm) > 0) {
					if(cursor->next != NULL) {
						cursor->next->prev = temp_node;
					}
					temp_node->prev = cursor;
					temp_node->next = cursor->next;
					cursor->next = temp_node;  
					return get_head(head);
				} else if (cursor->prev == NULL) {
					cursor->prev = temp_node;
					temp_node->next = cursor;
					return get_head(head);
				} else {
					cursor = cursor->prev;
				}
			}
		} else {/*no cases matched, we have a problem */
			printf("Could not determine sort order. Request failed.\n");
		}
	}
	}
}

/*return ptr to the node if we find a match */
struct node *search_ls(struct node* head, char *first, char *last, int name_order) {
	struct node *cursor = head;/*use this for iterating */

	if(cursor == NULL) {
		return;
	} else {
		if(name_order == LAST) {/*if user entered a last name */
			while(cursor != NULL) {/*make sure we can access list at this point */
				if(strcmp(cursor->last_nm, last) == 0) {
					return cursor;/*if strings match, return this node */
				} else {
					cursor = cursor->prev;/*otherwise, iterate */
				}
			}
		return NULL;
		} else if (name_order == FIRST) {     /*if user entered a first name */ 
			while(cursor != NULL) {/*make sure we can access list at this point */
				if(strcmp(cursor->first_nm, first) == 0) {
					return cursor;/*if strings match, return this node */
				} else {
					cursor = cursor->prev;/*otherwise, iterate */
				}
			}
		return NULL;
		} else {/*otherwise, we have a problem */
			printf("Could not use parameters given. Request failed.\n");
		}
	}
}

/*takes an int representing a node in the list, and returns that node */
struct node *get_nnode(struct node *head, int location) {
	int list_index;
	int list_length = get_ls_len(head);
	struct node* cursor = head;/*use this for iterating */
	if(head == NULL) {
		return head;
	} else {
		if(list_length >= location) {
			for(list_index = 1; list_index <= list_length; list_index++) {
				if(list_index == location) {/*if locations match, return this node */
					return cursor;
				} else {
					cursor = cursor->prev;/*otherwise, iterate */
				}
			}
		} else {/*otherwise, we have a problem */
			printf("Requested position was not found in the current list.");
		}
	}
}

/*returns the true head node of the list */
struct node *get_head(struct node *head) {
	struct node *cursor = head;/*use this for iterating */
	if (cursor == NULL) {
		printf("List provided is empty, nothing to do here...\n");
	} else {
		while(cursor->next != NULL) {/*if next ptr isnt null, we aren't at true head */
			cursor = cursor->next;/*so iterate */
		}
	}
	return cursor;/*return this node */
}

/*removed a node based on user input of first or last name. utilizes search method */
struct node *remove_node(struct node *head, char *first, char *last, int name_order) {
	struct node *remove_this = search_ls(head, first, last, name_order);/*use this to hold a node to remove */
	struct node *new_head = NULL;
	if(remove_this == NULL) {
		return NULL;
	} else {
		if(remove_this == get_head(head)) {
			new_head = remove_this->prev;
		}
		if(remove_this->next != NULL) {
			remove_this->next->prev = remove_this->prev;
		}
		if(remove_this->prev != NULL) {
			remove_this->prev->next = remove_this->next;
		}
		remove_this->next = NULL;
		remove_this->prev = NULL;
		free(remove_this->last_nm);/*make sure to free things from mem */
		free(remove_this->first_nm);/*make sure to free things from mem */
		free(remove_this);/*make sure to free things from mem */
		return new_head;
	}
}

/*return last node of the list */
struct node *get_tail(struct node *head) {
	if(head == NULL) {
		return head;
	} else {
		while(head->prev != NULL) {/*make sure we are at last */
			head = head->prev;/*iterate */
		}
	return head;
	}
}

/*returns integer represnting length of list */
int get_ls_len(struct node *head) {
	int node_length = 0;/*start at 0 and count */

	if(head == NULL) {
		return node_length;
	} else {
		while(head != NULL) {
			node_length++;/*add to count */
			head = head->prev;/*iterate*/
		}  
	}
	return node_length;/*return accumulation */
}

/*take a list and reverse it*/
struct node *reverse_ls(struct node *head) {
	if (head == NULL){/*make sure there is something to reverse */
		return NULL;
	}
	struct node *cursor = get_head(head);/*get only first node current iteration */
	while (cursor->prev != NULL) {/*while we still have nodes to deal with */
		struct node *temp = cursor->prev;
		cursor->prev = cursor->next;/*flip*/
		cursor->next = temp;/*flip*/
		cursor = temp; /*set current head to our temp node */
	}
	struct node *temp_node = cursor->prev;
	cursor->prev = cursor->next;/*flip*/
	cursor->next = temp_node;/*flip*/

	return cursor;
}

/*take a list and sort it by sort_order given */
struct node *sort_ls(struct node *head, int name_order, int sort_order) {
	if(head == NULL){/*make sure we have something to sort */
		return head;
	}
	struct node *temp = NULL;/*create a temp storage */
	while(head != NULL) {
		temp = add_node(temp, head->first_nm, head->last_nm, name_order, sort_order);/*add node to new list */
		head = remove_node(head, head->first_nm, head->last_nm, 1);/*make sure to clear (and free) the old one */
		if(head == NULL) {
			break;/*get out because we are done */
		}
	}
  return get_head(temp);
}
