
#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include <ctype.h> 
main(int argc, char *argv[]){
  int bytes;
  int n = 100;
  char *my_string; 
  my_string = (char *) malloc (n + 1);
  bytes = getline (&my_string, &n, stdin);
  if (bytes == -1){
      printf("ERROR!");
    }else{
	  char *my_string2 = my_string;
      printf("You typed:\n");
      puts (my_string);
	  printf("Uppercased is:");
	  int i, x;
	  char hold;
	  i = x = hold = 0;
	  int length = strlen(my_string2);
	  for(i = 0; i <= length - 1; i++){
		my_string2[i] = toupper(my_string[i]);
	  }
	  puts(my_string2);
    }
 return EXIT_SUCCESS;
}