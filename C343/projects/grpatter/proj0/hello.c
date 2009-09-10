/*Greg Patterson (grpatter)
C343 Project 0
*/
#include <stdio.h>

int main(){
  char name[80];
  int patience = 0;
  int wht_count = 0;
   prompt(name);
  printf("Hello, %s", name);
  puts("! \n\n");
  patience = getPatience(patience);
  //wht_count =  showSong(patience);//does NOT read from a file
  wht_count = readSong(patience);//DOES read from file
  printf("\nWait, you're leaving after only %d chunks of whitespace? %d lines.\n", wht_count, patience);
  return 0;
}

prompt(char *n){
	//take name into n
  	puts ("Please enter your name...");
	scanf("%s", n);
}

int getPatience(){
	int p = 0;
	puts("How patient are you? (In integer form) ... ");
	scanf("%d", &p);
	return p;
}

int showSong(int p){ //is this an acceptable solution rather than reading from a file?
int wht_count = 0;
if(p % 1 == 0){
	printf("This is the song that never ends,\n");
	wht_count = wht_count + 6;
}else if(p % 2 == 0){
	printf("Yes, it goes on and on, my friend.\n");
	wht_count = wht_count + 7;
}else if(p % 3 == 0){
	printf("Some people started singing it, not knowing what it was,\n");
	wht_count = wht_count + 9;
}else{
	printf("and they'll continue singing it forever just because...\n");
	wht_count = wht_count + 7;
}
return wht_count;
}

int readSong(int p){
	FILE *song = fopen("song.txt", "r");
	char ltr = '.';
	int r_count = 0;
	int wht_space = 0;
  
	while(p > 0)
	{
	  fscanf(song, "%c", &ltr);      
	  if(ltr == ' '){//found a white space
		wht_space++;
	  }
	  if(ltr == '\n'){//end of line
		p--;
		r_count++;
		wht_space++;
	  }      
	  if(r_count == 4){
		rewind(song); //last line of file, move to top
		r_count = 0;
	  }      
	  printf("%c", ltr);//print
	}  
	fclose(song);//make sure to close the file
	return wht_space;
}
