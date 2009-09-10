
#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include <ctype.h> /* so we can use the isblank method */
main(int argc, char *argv[])
{
int x = 5, i;
int *p;
int *q;
q = malloc(x*sizeof(int));
p = q + 4;
for (i = 0; i < x; i++){
 q[i] = i;
}
 printf("x is:%d and i is:%d\n",x,i);
printf("q points to:%d and p points to:%d.\n", *q,*p);
 return EXIT_SUCCESS;
}
