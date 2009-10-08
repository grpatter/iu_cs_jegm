/* Eric Spencer (erspence)
   A290 Unix and C, Spring 2009
   Linked-List project (main function)
   February 13th(Friday)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_struct.c"
#include "return_functions.c"

#define LAST 1 /*nodes last name*/
#define FIRST 0 /*nodes first name*/
#define ASCEND 1 /*goes from A-Z*/
#define DESCEND 0 /*goes from Z-A*/
int main(int argc, char *argv[]);
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
struct node *remove_node_handle(struct node *head){	
	char first_nm[24];
	char last_nm[24];
	
	printf("You have chosen to add a node to the list.\nPlease specify the first name: ");
	scanf("%23s",&first_nm);
	printf("Please specify the last name: ");
	scanf("%23s",&last_nm);
	return remove_node(head, first_nm, last_nm, 1);
}

struct node *search_ls_handle(struct node *head){	
	char first_nm[24];
	char last_nm[24];
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
	return search_list(head, first_nm, last_nm, name_o);
}
struct node *sort_ls_handle(struct node *head){
	int name_o = 0;
	int dir_o = 1;
	printf("\nIf you wish to sort by first name, enter 0. If you wish to insert by last name, enter 1: ");
	scanf("%d",&name_o);		
	printf("\nIf you wish to sort in descending (Z>A) order, enter 0. If you wish to insert in ascending (A>Z) order, enter 1: ");
	scanf("%d",&dir_o);
	return sort_list(head, name_o, dir_o);
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
				print_node_list(get_nth_node(head, loc));
				break;
			case 3: //return first element
				printf("Head element is the first of the following:\n");
				print_node_list(get_head_node(head));
				break;
			case 4: //return last element 				
				printf("Tail element is the first of the following:\n");
				print_node_list(get_tail_node(head));
				break;
			case 5: //remove element
				head = remove_node_handle(head);
				break;
			case 6: //search for element by either first or last name
				print_node_list(search_ls_handle(head));
				break;
			case 7: //reverse the list
				head = reverse_list(head);
				printf("List has been reversed, now looks like:\n");
				print_node_list(head);
				break;
			case 8: //sort by first or last in ascending or descending
				head = sort_ls_handle(head);
				break;
			case 9: //print list
				print_node_list(head);
				break;
			case 10: //reutrn length
				len = get_list_length(head);
				printf("Length of current list is:%d\n",len);
				break;
			case 11: //quit
				printf("Now exiting. Goodbye!\n");
				exit(0);
		}
		printf("\nPress any key to continue...");
		char cn = getchar();
		cn = getchar();
	}
}

int main(int argc, char *argv[]) {
  	struct node *cur_ls = NULL;
	show_menu(cur_ls);//go into menu
int last = 1;
  int first = 0;
  int ascend = 1;
  int descend = 0;

  // create a node to make sure that we can create one*/
  struct node* failure = NULL;
  /*call insert with an empty list*/
  failure = insert_node(failure, "greg", "patty", first, ascend);

  /*testing to print one-node list*/
  printf("\tTesting to print one-node list\n");
  print_node_list(failure);

  /*create nodes to put stuff in soon*/
  struct node* success = NULL;
  struct node* really = NULL;
  
  /*put another node in call it something new*/
  failure = insert_node(failure, "munir", "khabibullaev", first, ascend);

  /*testing to print two-node list*/
  printf("\tTesting to print two-node list in first, ascend order\n");
  print_node_list(failure);

  /*put another node in call it something new*/
  failure = insert_node(failure, "eric", "spencer", first, ascend);

  /*testing to print three-node list*/
  printf("\tTesting to print three-node list in first, ascend order\n");
  print_node_list(failure);

  /*testing the length of the list*/
  int length;
  length = get_list_length(failure);
  printf("\ttesting to check the length of the list, should be 3\n");
  printf("list length: %d\n\n", length);

  /*testing to print the nth node of the list*/
  printf("\tTesting to print get_nth_node function, we want 2nd should be greg\n");
  print_node_list(get_nth_node(failure,2));

  /*testing the length of the list after element nth node was called*/
  int length2;
  length2 = get_list_length(failure);
  printf("\ttesting to check the length of the list, should be 3\n");
  printf("list length: %d\n", length2);

  /*printing list again to check if greg was removed*/
  printf("\tTesting to see if the 2nd node was removed\n");
  print_node_list(failure);

  /*testing to print the tail_node of the list*/
  printf("\tTesting to print get_tail_node function, we want tail should be munir\n");
  print_node_list(get_tail_node(failure));

  /*add more elements into the list*/
  struct node *two_node_list = NULL;
  struct node *three_node_list = NULL;
  struct node *four_node_list = NULL;
  two_node_list = insert_node(failure, "breanne", "sinard", first, ascend);
  three_node_list = insert_node(two_node_list, "greg", "patterson", first, ascend);

  /*testing to print the list with four nodes*/
  printf("\tTesting to print list with four nodes\n");
  print_node_list(three_node_list);

  /*testing the get_head_node*/
  struct node *head_less = NULL;
  printf("\tTesting to print get_head_node function, we want head should be breanne\n");
  head_less = get_head_node(three_node_list);
  print_node_list(head_less);

  /*testing list again to check if breanne was removed*/
  printf("\tTesting to see if the head node was removed\n");
  print_node_list(head_less);

  /*searching list for last name to test search list function */
  printf("\tTesting to see if the search list function is working\n");
  print_node_list(search_list(head_less, "greg", "patterson", 1));

  /*searching list for last name to test search list function */
  printf("\tTesting to see if the search list function is working\n");
  print_node_list(search_list(head_less, "eric", "spencer", 0));

  /*printing the list before removing and arbitrary node*/
  printf("\tTesting the list before removing node\n");
  print_node_list(head_less);

  /*testing to see what the head node is*/
  printf("\tTesting to see what my head node is\n");
  print_node_list(get_head_node(head_less));

  /*removing an arbitrary node from the list*/
  printf("\tTesting to remove greg patterson\n");
  head_less = remove_node(head_less, "greg", "patterson", 1);
  head_less = remove_node(head_less, "eric", "spencer", 1);
  head_less = remove_node(head_less, "greg", "patty", 1);
  head_less = remove_node(head_less, "munir", "khabibullaev", 1);
  head_less = remove_node(head_less, "breanne", "sinard", 1);
  print_node_list(head_less);
  
  /*add more elements into the list*/
  struct node *reverse_node_list = NULL;
  reverse_node_list = insert_node(reverse_node_list, "george", "bush", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "eric", "spencer", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "peter", "pan", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "brooke", "burke", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "robin", "williams", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "david", "gates", last, descend);

  /*printing the list before reversing*/
  printf("\tTesting the list before reversing list\n");
  print_node_list(reverse_node_list);
  
  /*checking the head node*/
  printf("\tChecking the head node\n");
  print_node_list(get_head_node(reverse_node_list));

  /*reversing the list*/
  reverse_node_list = reverse_list(reverse_node_list);

    /*checking the head node*/
  printf("\tChecking the head node\n");
  print_node_list(get_head_node(reverse_node_list));

  /*printing the list after reversing*/
  printf("\tTesting the list after reversing\n");
  print_node_list(reverse_node_list);

  /*reversing the list*/
  reverse_node_list = reverse_list(reverse_node_list);

  /*printing the list after reversing*/
  printf("\tTesting the list after reversing\n");
  print_node_list(reverse_node_list);

  struct node *sorted_list = NULL;
  /*printing the list by first name in ascending order*/
  sorted_list = sort_list(reverse_node_list, first, ascend);
  printf("printing the list by first name in ascending order\n");
  print_node_list(sorted_list);

  /*printing the list by last name in ascending order*/
  sorted_list = sort_list(reverse_node_list, last, ascend);
  printf("printing the list by last name in ascending order\n");
  print_node_list(sorted_list);

  /*printing the list by last name in descending order*/
  sorted_list = sort_list(reverse_node_list, last, descend);
  printf("printing the list by last name in ascending order\n");
  print_node_list(sorted_list);

  /*printing the list by first name in descending order*/
  sorted_list = sort_list(reverse_node_list, first, descend);
  printf("printing the list by first name in descending order\n");
  print_node_list(sorted_list);

  return 0;
}
