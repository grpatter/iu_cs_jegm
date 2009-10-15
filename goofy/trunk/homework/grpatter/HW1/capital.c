
#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include <ctype.h> /* so we can use the isblank method */
main(int argc, char *argv[])
{
  while((argc = getchar()) != EOF){ /*continue while not at end of file char */
	if(argc != '\n'){
		printf("read:%c becomes:%c\n", argc, toupper(argc));
	}
 }
 return EXIT_SUCCESS;
}