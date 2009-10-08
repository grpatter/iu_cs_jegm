/* Greg Patterson (grpatter)
	A290
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_struct.c"
#include "return_functions.c"

#define LAST 1 /*last name*/
#define FIRST 0 /*first name*/
#define ASCEND 1 /*for A-Z order*/
#define DESCEND 0 /*for Z-A order*/
/*START prototypes*/
int main(int argc, char *argv[]);
void show_menu(struct node* head);
struct node *add_node_handle(struct node *head);
void print_node_list(struct node *head);
/*END prototypes*/

struct node *add_node_handle(struct node *head){	
	char first_nm[24];/*to hold first name*/
	char last_nm[24];/*to hold last name*/
	int name_o = 0;/*to hold name type for insert*/
	int dir_o = 1;/*to hold name direction for insert*/
	
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
struct node *remove_node_handle(struct node *head){	
	char first_nm[24];/*to hold first name*/
	char last_nm[24];/*to hold last name*/
	
	printf("You have chosen to add a node to the list.\nPlease specify the first name: ");
	scanf("%23s",&first_nm);
	printf("Please specify the last name: ");
	scanf("%23s",&last_nm);
	return remove_node(head, first_nm, last_nm, 1);
}

struct node *search_ls_handle(struct node *head){	
	char first_nm[24];/*to hold first name*/
	char last_nm[24];/*to hold last name*/
	int name_o;
	printf("If you are specifying first name, enter 0. If you are specifying last name, enter 1: ");
	scanf("%d",&name_o);
	if(name_o == 0){
		//first name		
		printf("Please enter the first name: ");
		scanf("%23s",&first_nm);
	}else{
		//last name
		printf("Please enter the last name: ");
		scanf("%23s",&last_nm);
	}
	return search_ls(head, first_nm, last_nm, name_o);
}
struct node *sort_ls_handle(struct node *head){
	int name_o = 0;/*to hold name type for sorting*/
	int dir_o = 1;/*to hold name direction for sorting*/
	printf("\nIf you wish to sort by first name, enter 0. If you wish to insert by last name, enter 1: ");
	scanf("%d",&name_o);		
	printf("\nIf you wish to sort in descending (Z>A) order, enter 0. If you wish to insert in ascending (A>Z) order, enter 1: ");
	scanf("%d",&dir_o);
	return sort_ls(head, name_o, dir_o);
}

void print_node_list(struct node *head) {
	printf("Printing current list below.\nFormat: NODE:<first>, <last>\n\n");
  if(head == NULL) {/*nothing to print if nothing in list */
    printf("Head is null. Nothing to do here...\n");
  } else {
    while(head != NULL) {
      printf("NODE: %s, %s\n", head->first_nm, head->last_nm);
      head = head->prev;/*iterate to next node */
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
				printf("*****Add Node Action Started*****\n");
				head = add_node_handle(head);
				printf("*****Add Node Action Finished*****\n");
				break;
			case 2: //return element n
				printf("*****Get Nth Element Action Started*****\n");
				printf("Please enter the number corresponding to element you would like returned:");
				scanf("%d",&loc);	
				if(loc > get_ls_len(head)){
					printf("Integer given not valid for this list. Aborting.\n");
					break;
				}
				printf("Nth element is the first of the following:\n");
				print_node_list(get_nnode(head, loc));
				printf("\n*****Get Nth Element Action Finished*****\n");
				break;
			case 3: //return first element
				printf("*****Get First Element Action Started*****\n");
				printf("Head element is the first of the following:\n");
				print_node_list(get_head(head));
				printf("\n*****Get First Element Action Finished*****\n");
				break;
			case 4: //return last element 	
				printf("*****Get Last Element Action Started*****\n");			
				printf("Tail element is the first of the following:\n");
				print_node_list(get_tail(head));
				printf("\n*****Get Last Element Action Finished*****\n");
				break;
			case 5: //remove element
				printf("*****Remove Node Action Started*****\n");
				head = remove_node_handle(head);
				printf("*****Remove Node Action Finished*****\n");
				break;
			case 6: //search for element by either first or last name
				printf("*****Search List Action Started*****\n");
				print_node_list(search_ls_handle(head));
				printf("*****Search List Action Finished*****\n");
				break;
			case 7: //reverse the list
				printf("*****Reverse List Action Started*****\n");
				head = reverse_ls(head);
				printf("List has been reversed, now looks like:\n");
				print_node_list(head);
				printf("*****Reverse List Action Finished*****\n");
				break;
			case 8: //sort by first or last in ascending or descending
				printf("*****Sort List Action Started*****\n");
				head = sort_ls_handle(head);
				printf("*****Sort List Action Finished*****\n");
				break;
			case 9: //print list
				printf("*****Print List Action Started*****\n");
				print_node_list(head);
				printf("*****Print List Action Finished*****\n");
				break;
			case 10: //return length
				printf("*****Get Length Action Started*****\n");
				len = get_ls_len(head);
				printf("Length of current list is:%d\n",len);
				printf("*****Get Length Action Finished*****\n");
				break;
			case 11: //quit
				printf("Now exiting. Goodbye!\n");
				exit(0);
		}
		printf("\nPress any key to continue...");
		char cn = getchar();
		cn = getchar();/*must use two since we (oddly) recieve one automaticcally */
	}
}

int main(int argc, char *argv[]) {
  	struct node *cur_ls = NULL;
	show_menu(cur_ls);//go into menu
  return 0;
}
