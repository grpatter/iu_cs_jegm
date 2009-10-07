#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include<string.h>
#include <ctype.h> 
#define TRUE 1
#define FALSE 0 
main(int argc, char *argv[]){
  int bytes;
  int n = 100;
  char *my_string;
  int isPalindrome = TRUE; 

  printf("Please enter a line of text.\n");
  my_string = (char *) malloc (n + 1);
  bytes = getline (&my_string, &n, stdin);
  if (bytes == -1){
      printf("ERROR!");
    }else{
      printf("You typed:\n");
      puts (my_string);
	  
	  int i, x;
	  i = 0;
	  x = strlen(my_string) - 2;
	  while(i <= x){
		if(my_string[i] != my_string[x]){
			isPalindrome = FALSE;
			break;
		}
		i++;
		x--;
	  }
	  if(isPalindrome){
		puts("Palindrome.");
	  }else{
		puts("Not Palindrome.");
	  }
    }
  return EXIT_SUCCESS;
}