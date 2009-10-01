#include <bsp/ggbsp.h>
#include <stdio.h>
int  main(){
  gg_init(5);
 delay_125uS(2000);
   
  IO1DIR |= 1<<16;
  IO0DIR |= 1<<1;
  IO0DIR |= 1<<3;
  IO1DIR |= 1<<25;
  IO0DIR |= 1<<7;
  IO1DIR |= 1<<23;

  while(1){
    if (IO0PIN & 1 << 16){
      delay_125uS(2000);
      delay_125uS(2000);
      IO1SET = 1<<16;
      delay_125uS(2000);
      IO0SET = 1<<1;
      delay_125uS(2000);
      IO0SET = 1<<3;
      delay_125uS(2000);
      IO1SET = 1<<25;
      delay_125uS(2000);
      IO0SET = 1<<7;
      delay_125uS(2000);
      IO1SET = 1<<23;
      delay_125uS(2000);
    }else{
      delay_125uS(2000);
      delay_125uS(2000);
      IO1SET = 1<<23;
      delay_125uS(2000);
      IO0SET = 1<<7;
      delay_125uS(2000);
      IO1SET = 1<<25;
      delay_125uS(2000);
      IO0SET = 1<<3;
      delay_125uS(2000);
      IO0SET = 1<<1;
      delay_125uS(2000);
      IO1SET = 1<<16;
      delay_125uS(2000);
    }

  IO1CLR = 1<<16;
  IO0CLR = 1<<1;
  IO0CLR = 1<<3;
  IO1CLR = 1<<25;
  IO0CLR = 1<<7;
  IO1CLR = 1<<23;
   
  }
  return 0;
}
