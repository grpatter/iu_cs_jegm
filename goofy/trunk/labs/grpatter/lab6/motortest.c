#include <bsp/ggbsp.h>
#include <stdio.h>
int  main(){
  gg_init(6);
  printf("Setting DIRs\n");
  IO0DIR |= 1<<21;
  IO0DIR |= 1<<20;
  IO0DIR |= 1<<0;
  IO1DIR |= 1<<19;

while(1){
  printf("Motors OFF\n");
  IO0CLR = 1<<21;
  IO0CLR = 1<<20;
  IO0CLR = 1<<0;
  IO1CLR = 1<<19;

  delay_125uS(8000);
  printf("Motors ON. Watch me run FORWARD!\n");
  IO0SET = 1<<21;
  IO0SET = 1<<20;
  IO0SET = 1<<0;
  IO1SET = 1<<19;
  delay_125uS(8000);
	 
  printf("Clearing Motors. Watch me stand here!\n");
  IO0CLR = 1<<21;
  IO0CLR = 1<<20;
  IO0CLR = 1<<0;
  IO1CLR = 1<<19;
  delay_125uS(8000); 
	
  printf("Motors ON. Watch me run BACKWARD!\n");
  IO0SET = 1<<21;
  IO0SET = 1<<20;
  IO0SET = 1<<0;
  IO1SET = 1<<19;
  IO0CLR = 1<<20;
  IO1CLR = 1 <<19; 
  delay_125uS(8000);

  printf("Clearing Motors. Watch me stand here!\n");
  IO0CLR = 1<<21;
  IO0CLR = 1<<20;
  IO0CLR = 1<<0;
  IO1CLR = 1<<19;
  delay_125uS(8000);

  
  printf("Motors ON. Watch me pirouette right!\n");
  IO0SET = 1<<21;
  IO0SET = 1<<20;
  IO0SET = 1<<0;
  IO1SET = 1<<19;
  IO1CLR = 1<<19;
  delay_125uS(8000); 

  printf("Clearing Motors. Watch me stand here!\n");
  IO0CLR = 1<<21;
  IO0CLR = 1<<20;
  IO0CLR = 1<<0;
  IO1CLR = 1<<19;
  delay_125uS(8000);
 
  printf("Motors ON. Watch me pirouette left!\n");
  IO0SET = 1<<21;
  IO0SET = 1<<20;
  IO0SET = 1<<0;
  IO1SET = 1<<19;
  IO0CLR = 1<<20;
  delay_125uS(8000);

  printf("Clearing Motors. All done!\n");
  IO0CLR = 1<<21;
  IO0CLR = 1<<20;
  IO0CLR = 1<<0;
  IO1CLR = 1<<19;
  delay_125uS(8000);


}
  
  return 0;
}
