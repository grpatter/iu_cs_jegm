#include <stdio.h>
void main(){
  char c;
  int chars, lines, words;
  lines = words = chars = 0;
  while((c = getchar()) != EOF){
    if(c == '\n'){
      lines++;
      words++;
    }else if(c == ' '){
      words++;
    }else if(c == '\t'){
      words++;
    }else{
      chars++;
    }
  }
  printf("lines:%d   words:%d   chars:%d\n",lines,words,chars);
  return 0;
}
