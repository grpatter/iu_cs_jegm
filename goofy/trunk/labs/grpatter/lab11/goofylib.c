#include "channel.h"
#include <bsp/ggbsp.h>
#include <stdio.h>
#include "gg_music.h"

int msg = -1;
unsigned int timer = 0;
unsigned int counter_500mS = 0;
static int music_ind = -1;
int mfile = 0;

//receives valid byes from the buffer msg
//and puts the resulting goofy funtion to standard output

void mv_dirs(void) {
	IO0DIR |= 0x1 << 21;
	IO0DIR |= 0x1 << 20;
	IO0DIR |= 0x1;
	IO1DIR |= 0x1 << 19;
}
void mv_forward(void){
	IO0DIR |= 1<<21;//set pin DIRs
	IO0DIR |= 1<<20;
	IO0DIR |= 1<<0;
	IO1DIR |= 1<<19;
	printf("set pin DIRs\n");

	IO0CLR = 1<<21;//make sure all CLR
	IO0CLR = 1<<20;
	IO0CLR = 1<<0;
	IO1CLR = 1<<19;
	printf("cleared pins \n");
	IO0SET = 1<<21;//set in FORWARD motion
	IO0SET = 1<<20;
	IO0SET = 1<<0;
	IO1SET = 1<<19;
	printf("tried to set FORWARD\n");

}
void mv_rotate(void){
	IO0DIR |= 1<<21;//set pin DIRs
	IO0DIR |= 1<<20;
	IO0DIR |= 1<<0;
	IO1DIR |= 1<<19;
	printf("set pin DIRs\n");

	IO0CLR = 1<<21;//make sure all CLR
	IO0CLR = 1<<20;
	IO0CLR = 1<<0;
	IO1CLR = 1<<19;
	printf("cleared pins \n");
	IO0SET = 1<<21;//set to ROTATE
	IO0SET = 1<<20;
	IO0SET = 1<<0;
	IO1SET = 1<<19;
	IO1CLR = 1<<19;
	printf("tried to set ROTATE\n");
}
void mv_stop(void){
	IO0CLR = 1<<21;//CLR before exit
	IO0CLR = 1<<20;
	IO0CLR = 1<<0;
	IO1CLR = 1<<19;
	printf("exiting mv_motors\n");
}

int message_handler() {
  static int state = 0;
  static int data;

  Begin();
  mv_dirs();

  while(1) {
    Receive(msg, data);
    switch (data) {      
    case 0xA1:
      counter_500mS = 0;
      state = 1;
      printf("State: Forward\n");
      mv_forward();
      break;
    case 0xB1:
      counter_500mS = 0;
      printf("State: Music\n");
      music_ind = 0;
      mfile ^= 1;
      state = 2;
      break;
    case 0xC1:
      counter_500mS = 0;
      printf("State: Pirouetting\n");
      state = 3;
      mv_rotate();
      break;
    case 0xD1:      
      IO1DIR |= 0x3 << 22;
      IO1SET = 0x3 << 22;
      IO1CLR = 0x3 << 22;
      counter_500mS = 0;
      printf("State: Release\n");
	if(state == 2){
		state = 0;
	}else{
		mv_stop();
		state = 0;
	}
      break;
    }
  }
  return 0;
}

int message_decoder(){
  static unsigned int message = 0;
  static int zero = 0;
  static int one = 0;
  static int totalBits = 0;
  static int previous = 1;
  static int c;
  
  Begin();
  c = (IO0PIN >>18) & 1;
  if (zero > 30 && c == 1 && previous == 0){
    message = 0;
    zero = 0;
    one = 1;
    totalBits = 0;
  }
  if ( c == 0 ){
    zero ++;
  }
  if (c == 1){
    if(previous == 0){
      if (zero > one){
	if(totalBits != 0){
	  message = message << 1;
	}
      }
      else{
	if(totalBits != 0){
	  message = ((message << 1)|0x01);
	}
      }
      zero = 0;
      one = 1;
      totalBits++;
    }
    else{
      one++;
    }
  }
  if ( totalBits > 8){
    Send(msg, message);
    message = 0;
    zero = 0;
    one = 0;
    totalBits = 0;
  }
  previous = c;
  Finish();
}

void timer1_handler(void) __attribute__ ((interrupt ("IRQ")));
void null_handler(void) __attribute__ ((interrupt ("IRQ")));

void vic_setup (void) {
  VICInit(&null_handler);
  VICSetup(TIMER1,MIN_VIC_PRIORITY-2,&timer1_handler,0);
}

void null_handler(void) {  VICVectAddrRead=0x00; }

void timer1_setup(void) {
  T1MR0=7499;  // Timer counts from 0 to 7499. This gives 7500*(1/60mhz)=125uS 
  T1MCR=0x3;   // Reset counter on match with T1MR0 and generate interrupt 
  T1TCR=1;     // Enable TIMER1
}

void play_sound(){
	static int tm = 0;//to make sure we play at right speed
	if(music_ind == -1){//if not changed from button press, do nothing
		return;
	}
	if(music_ind < sizeof(sound_0)){
		if(tm ^= 1){
		DACR = sound_0[music_ind] << 8; // must be aligned on byte 2
		//delay_125uS(2);
		music_ind++;//increment for next pass
		}
	}else{
	  music_ind = -1;//reset
	  return;//get out
	}
}

void timer1_handler(void) {
  message_decoder();
  play_sound();
  T1IR=1;     // Write a 1 to the timer1 interrupt register to clear the T1MR0 match
  if (counter_500mS++ > 4000) {
    M1_OFF;//set to off state
    M2_OFF;
    counter_500mS = 0;//reset timer
  }
  VICVectAddrRead=0x00;
}
