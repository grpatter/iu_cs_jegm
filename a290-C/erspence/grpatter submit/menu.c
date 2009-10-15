/* Eric Spencer (erspence)
   A290 Unix and C, Spring 2009
   Linked-List project (user menu functions)
   February 24 (Tuesday)
*/

#include <stdio.h>
#include <stdlib.h>
void show_menu(struct node* head); /*prototype*/
struct node *add_node_handle(struct node *head);
void print_node_list(struct node *head);

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
	return insert_node(head, first_nm, last_nm, name_o, dir_o);
}

void print_node_list(struct node *head) {
  if(head == NULL) {
    printf("go directly to jail. do not pass go, do not collect $200\n");
  } else {
    while(head != NULL) {
      printf("first: %s, last: %s\n", head->first_name, head->last_name);
      head = head->previous_node;
    }
  }
}

void show_menu(struct node* head){
	int menu, len, loc;
	char cont;	
	while(1){
		printf("\n\n");
		printf("You may take any of the following actions.\n");
		printf("1- Add a value to the list \n");
		printf("2- Return the nth element in the list. \n");
		printf("3- Return the first element in the list. \n");
		printf("4- Return the last element in the list. \n");
		printf("5- Remove an arbitrary element in the list. \n");
		printf("6- Search for an arbitrary element by either first or last name. \n");
		printf("7- Reverse the list. \n");
		printf("8- Sort the list by either first or last name, in either ascending or descending order. \n");
		printf("9- Print the list. \n");
		printf("10- Return the length of the list. \n");
		printf("11- Quit. \n");
		printf("Please enter the number corresponding to the action you would like to take:");
		scanf("%d",&menu);
		printf("\n");
		
		switch(menu){
			case 1: //add value
				head = add_node_handle(head);
				break;
			case 2: //return element n
				printf("Please enter the number corresponding to element you would like returned:");
				scanf("%d",&loc);				
				printf("Nth element is the first of the following:\n");
				print_node_list(get_nnode(head, loc));
				break;
			case 3: //return first element
				printf("Head element is the first of the following:\n");
				print_node_list(get_head(head));
				break;
			case 4: //return last element 				
				printf("Tail element is the first of the following:\n");
				print_node_list(get_tail(head));
				break;
			case 5: //remove element
				rem_element(head);
				break;
			case 6: //search for element by either first or last name
				//search_ls();
				break;
			case 7: //reverse the list
				head = rev_ls(head);
				break;
			case 8: //sort by first or last in ascending or descending
				//sort_ls();
				break;
			case 9: //print list
				print_node_list(head);
				break;
			case 10: //reutrn length
				len = ret_len(head);
				printf("Length of current list is:%d\n",len);
				break;
			case 11: //quit
				printf("Now exiting. Goodbye!\n");
				exit(0);
		}
	}
}
/*
void initial_menu() {
  printf("Welcome, you have enetered the wonderful world of FUN WITH POINTERS.");
  printf("\tPlease select an option from the menu.");
}
*/

