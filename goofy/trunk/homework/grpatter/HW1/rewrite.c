
#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include <ctype.h> /* so we can use the isblank method */
main(int argc, char *argv[])
{
 i = 0;
c = getchar();
while (c != ’ ’ && c != ’\n’ && c != ’\t’ && c != EOF) {
word[i++] = c;
c = getchar();
}
 return EXIT_SUCCESS;
}