
#include <stdio.h> /*standard input/output lib */
#include <stdlib.h> 
#include <ctype.h> /* so we can use the isblank method */
main(int argc, char *argv[])
{
 int i;
 int sum = 0;
 int min = atoi(argv[1]);
 int max = atoi(argv[1]);
 int *ip; //num array
 ip = (int *) malloc(argc*sizeof(int));
	for(i = 1; i < argc; i++){
		int tmp = atoi(argv[i]);
		ip[i-1] = tmp;
		sum = sum + tmp;
		if(tmp < min){
			min = tmp;
		}
		if(tmp > max){
			max = tmp;
		}
	}
	printf("Sum: %d\nMin: %d\nMax: %d\n",sum,min,max);
	/* release the memory when we're done */
	free(ip);
 return EXIT_SUCCESS;
}
