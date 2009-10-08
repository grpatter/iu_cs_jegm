
#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include <ctype.h> /* so we can use the isblank method */
main(int argc, char *argv[])
{
  int bytes;
  int n = 100;
  char *my_string;

  printf("Please enter a line of text.\n");
  /* These 2 lines are the heart of the program. */
  my_string = (char *) malloc (n + 1);
  bytes = getline (&my_string, &n, stdin);

  if (bytes == -1)
    {
      printf("ERROR!");
    }
  else
    {
	  char *my_string2 = my_string;
      printf("You typed:\n");
      puts (my_string);
	  printf("Reverse is:");
	  int i, x;
	  char hold;
	  i = x = hold = 0;
	  int length = strlen(my_string2);
	  for(i = 0, x = length - 1; i <= x; i++, x--){
		hold = my_string2[i];
		my_string2[i] = my_string2[x];
		my_string2[x] = hold;
	  }
	  puts(my_string2);
    }
  return EXIT_SUCCESS;
}