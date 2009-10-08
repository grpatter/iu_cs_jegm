int last = 1;
  int first = 0;
  int ascend = 1;
  int descend = 0;

  // create a node to make sure that we can create one*/
  struct node* failure = NULL;

  printf("\tTesting to print NULL list\n");
  print_node_list(failure);

  // call insert with an empty list*/
  failure = insert_node(failure, "greg", "patty", first, ascend);

  // testing to print one-node list*/
  printf("\tTesting to print one-node list\n");
  print_node_list(failure);

  // create nodes to put stuff in soon*/
  struct node* success = NULL;
  struct node* really = NULL;
  
  // put another node in call it something new*/
  failure = insert_node(failure, "munir", "khabibullaev", first, ascend);

  // testing to print two-node list*/
  printf("\tTesting to print two-node list in first, ascend order\n");
  print_node_list(failure);

  // put another node in call it something new*/
  failure = insert_node(failure, "eric", "spencer", first, ascend);

  // testing to print three-node list*/
  printf("\tTesting to print three-node list in first, ascend order\n");
  print_node_list(failure);

  // testing the length of the list*/
  int length;
  length = get_list_length(failure);
  printf("\ttesting to check the length of the list, should be 3\n");
  printf("list length: %d\n\n", length);

  // testing to print the nth node of the list*/
  printf("\tTesting to print get_nth_node function, we want 2nd should be greg\n");
  print_node_list(get_nth_node(failure,2));

  // testing the length of the list after element nth node was called*/
  int length2;
  length2 = get_list_length(failure);
  printf("\ttesting to check the length of the list, should be 3\n");
  printf("list length: %d\n", length2);

  // printing list again to check if greg was removed*/
  printf("\tTesting to see if the 2nd node was removed\n");
  print_node_list(failure);

  // testing to print the tail_node of the list*/
  printf("\tTesting to print get_tail_node function, we want tail should be munir\n");
  print_node_list(get_tail_node(failure));

  // add more elements into the list*/
  struct node *two_node_list = NULL;
  struct node *three_node_list = NULL;
  struct node *four_node_list = NULL;
  two_node_list = insert_node(failure, "breanne", "sinard", first, ascend);
  three_node_list = insert_node(two_node_list, "greg", "patterson", first, ascend);

  // testing to print the list with four nodes*/
  printf("\tTesting to print list with four nodes\n");
  print_node_list(three_node_list);

  // testing the get_head_node*/
  struct node *head_less = NULL;
  printf("\tTesting to print get_head_node function, we want head should be breanne\n");
  head_less = get_head_node(three_node_list);
  print_node_list(head_less);

  // testing list again to check if breanne was removed*/
  printf("\tTesting to see if the head node was removed\n");
  print_node_list(head_less);

  // searching list for last name to test search list function */
  printf("\tTesting to see if the search list function is working\n");
  print_node_list(search_list(head_less, "greg", "patterson", 1));

  // searching list for last name to test search list function */
  printf("\tTesting to see if the search list function is working\n");
  print_node_list(search_list(head_less, "eric", "spencer", 0));

  // printing the list before removing and arbitrary node*/
  printf("\tTesting the list before removing node\n");
  print_node_list(head_less);

  // testing to see what the head node is*/
  printf("\tTesting to see what my head node is\n");
  print_node_list(get_head_node(head_less));

  // removing an arbitrary node from the list*/
  printf("\tTesting to remove greg patterson\n");
  head_less = remove_node(head_less, "greg", "patterson", 1);
  head_less = remove_node(head_less, "eric", "spencer", 1);
  head_less = remove_node(head_less, "greg", "patty", 1);
  head_less = remove_node(head_less, "munir", "khabibullaev", 1);
  head_less = remove_node(head_less, "breanne", "sinard", 1);
  print_node_list(head_less);
  
  // add more elements into the list*/
  struct node *reverse_node_list = NULL;
  reverse_node_list = insert_node(reverse_node_list, "george", "bush", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "eric", "spencer", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "peter", "pan", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "brooke", "burke", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "robin", "williams", last, descend);
  reverse_node_list = insert_node(reverse_node_list, "david", "gates", last, descend);

  // printing the list before reversing*/
  printf("\tTesting the list before reversing list\n");
  print_node_list(reverse_node_list);
  
  // checking the head node*/
  printf("\tChecking the head node\n");
  print_node_list(get_head_node(reverse_node_list));

  // reversing the list*/
  reverse_node_list = reverse_list(reverse_node_list);

    // checking the head node*/
  printf("\tChecking the head node\n");
  print_node_list(get_head_node(reverse_node_list));

  // printing the list after reversing*/
  printf("\tTesting the list after reversing\n");
  print_node_list(reverse_node_list);

  // reversing the list*/
  reverse_node_list = reverse_list(reverse_node_list);

  // printing the list after reversing*/
  printf("\tTesting the list after reversing\n");
  print_node_list(reverse_node_list);

  struct node *sorted_list = NULL;
  // printing the list by first name in ascending order*/
  sorted_list = sort_list(reverse_node_list, first, ascend);
  printf("printing the list by first name in ascending order\n");
  print_node_list(sorted_list);

  // printing the list by last name in ascending order*/
  sorted_list = sort_list(reverse_node_list, last, ascend);
  printf("printing the list by last name in ascending order\n");
  print_node_list(sorted_list);

  // printing the list by last name in descending order*/
  sorted_list = sort_list(reverse_node_list, last, descend);
  printf("printing the list by last name in ascending order\n");
  print_node_list(sorted_list);

  // printing the list by first name in descending order*/
  sorted_list = sort_list(reverse_node_list, first, descend);
  printf("printing the list by first name in descending order\n");
  print_node_list(sorted_list);