/*Greg Patterson (grpatter)
C343 Project 0
*/
#include <stdio.h>



int main(){
  char name[80];
   prompt(name);
  printf("Hello, %s", name);
  puts("! \n");
  int patience;
  getPatience(patience);
  showSong(patience);
  return 0;
}

prompt(char *n){
	//take name into n
  	puts ("Please enter your name...");
	scanf("%s", n);
}

getPatience(int p){
	puts("How patient are you? (In integer form) ... ");
	scanf("%d", &p);
}

showSong(int p){
if(p % 1 == 1){
	printf("This is the song that never ends,\n");
}else if(p % 2 == 1){
	printf("Yes, it goes on and on, my friend.\n");
}else if(p % 3 == 1){
	printf("Some people started singing it, not knowing what it was,\n");
}else{
	printf("and they'll continue singing it foever just because...\n");
}
}
