
#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include <ctype.h> /* so we can use the isblank method */
main(int argc, char *argv[])
{
int x = 5, y = 6;
int *p;
int *q;
p = &x;//5
q = &y;//6
x = 2*(*q) + *p;
q = p;
*q = x + y;
printf("x is:%d and y is:%d\n",x,y);
 return EXIT_SUCCESS;
}