#include <bsp/ggbsp.h>
#include <stdio.h>
int  main(){
  char c;
  gg_init(5);
  while(1){
    delay_125uS(2000);
    //turn on DS1
    printf("trying to turn ON DS1\n");
    IO1DIR |= 1<<16;
    IO1SET = 1<<16;
    delay_125uS(2000);
    //turn off DS1
    printf("trying to turn OFF DS1.\n");
    IO1CLR = 1<<16;
  }
  return 0;
}
//DS1 P1.16
//pin #1 bit 16 (#17)
