#include <bsp/ggbsp.h>
#include "myprintf.c"
int  main(){
  char c;
  int chars, lines, words;
  gg_init(1);
  lines = words = chars = 0;
  while((c = getchar()) != 0x1b){ /*end on esc */
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
  myprintf("lines:%d   words:%d   chars:%d\n",lines,words,chars);
   //printstring("lines");
  //printint(lines);
  //printint(words);
  //printint(chars);
  return 0;
}
