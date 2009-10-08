
#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include <ctype.h> /* so we can use the isblank method */
main(int argc, char *argv[])
{

char *animals[] = {"Bear\0", "Bat\0", "Tiger\0", "Cat\0"}; 
//animals[3][0], animals[0][11], animals[3][-6]
printf("animals[3][0]:%s\n",
animals[3][0]);

 return EXIT_SUCCESS;
}
