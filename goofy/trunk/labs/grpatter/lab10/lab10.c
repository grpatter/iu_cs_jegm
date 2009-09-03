#include <bsp/ggbsp.h>
#include "channel.h"

// Motors
#define M1_EN 21 // P0
#define M1_DIR 20 // P0
#define M2_EN 0 // P0
#define M2_DIR 19 // P1
#define M1_ON IO0SET=(1<<M1_EN)
#define M1_OFF IO0CLR=(1<<M1_EN)
#define M1_FORWARD IO0SET=(1<<M1_DIR)
#define M1_BACKWARD IO0CLR=(1<<M1_DIR)
#define M2_ON IO0SET=(1<<M2_EN)
#define M2_OFF IO0CLR=(1<<M2_EN)
#define M2_FORWARD IO1SET=(1<<M2_DIR)
#define M2_BACKWARD IO1CLR=(1<<M2_DIR)
#define FULLSPEED 100;

int msg = -1;
int stop = 0;
static int state = 0;//0 = stop, 1 = for, 2 = right, 3 = left
static int spinspd = 0;
static int time = 0;


void dance(int type)
{
	M1_ON;
	M2_ON;
	PWMMR5 = -127;
	PWMMR1 = 127;
}

void left_power(int val){
	M1_ON;
	if(val < 0){
		M1_BACKWARD;
		val *= -1;
	}else{
		M1_FORWARD;
	}if(val > 127){
		PWMMR5 = 127;
	}else{
		PWMMR5 = val;
	}
}
void right_power(int val){
	M2_ON;
	if(val < 0){
		M2_BACKWARD;
		val *= -1;
	}else{
		M2_FORWARD;
	}
	if(val > 127){
		PWMMR1 = 127;
	}else{
		PWMMR1 = val;
	}
}

void stop_clear(){
	//IO0CLR = 1<<21;//make sure all CLR
	//IO0CLR = 1<<20;
	//IO0CLR = 1<<0;
	//IO1CLR = 1<<19;
	M1_OFF;
	M2_OFF;
	PWMMR5 = 0;
	PWMMR1 = 0;
	state = 0; //reset this
	time = 0;
}

void go_forward(){
	int spd = FULLSPEED;
	left_power(spd);
	right_power(spd);
	time = 0;
}

void spin_left(int spin){
	left_power(spin);
	right_power(127);
	time = 0;
}
void spin_right(int spin){
	left_power(127);
	right_power(spin);
	time = 0;
}

int message_handler()
{
  static int data;
  Begin();
  while (1){
	time++;
	if((state == 2 || state == 3) && (time>60)){
		printf("*************Timeout occurred...\n");
		state = 1;
		//stop_clear();
		go_forward();
	}
    Receive(msg, data);
    if(data == 0xA1) {//forward button
      //if state = 2 spin more otherwise forward
	printf("Forward Button..state:%i\n",state);
	if(state == 0){
		state = 1;
		go_forward();
		spinspd = FULLSPEED;
	}else {
		state = 2;
		spinspd = spinspd - 5;
		printf("Calling spin_right with spinspd:%i\n",spinspd);
		spin_right(spinspd);
	}
    }
    if(data == 0xB1){//music button
	printf("Stop Button..state:%i\n",state);
	state = 0;
	spinspd = 0;
 	stop_clear();
    }
    if(data == 0xC1){//spin button
	//if state = 3 spin more otherwise forward
	printf("Rotate Button..state:%i\n",state);
	if(state == 0){
		state = 1;
		go_forward();
		spinspd = FULLSPEED;
	}else{
		state = 3;
		spinspd = spinspd - 5;
		printf("Calling spin_left with spinspd:%i\n",spinspd);
		spin_left(spinspd);
	}
    }
    if(data == 0xD1){//no button
	printf("Released Buttons..state:%i\n",state);
	if(state == 0){
		//nothing
		stop_clear();
	}else{
		state = 1;//forward state
		spinspd = FULLSPEED;
		go_forward();
	}
    }
    if((IO0PIN >> 15) & 1){
      if(data == 0xA1 || data == 0xB1 || data == 0xC1 || data == 0xD1){
	dance(1);
	dance(1);
	dance(1);
	dance(1);
      }
    }
  }
}

int message_decoder()
{
  static unsigned int message = 0;
  static int zero = 0;
  static int one = 0;
  static int totalBits = 0;
  static int previous = 1;
  static int c;
  
  Begin();
  //  while ((c = getchar()) != EOF)
  // while(1){
  c = (IO0PIN >>18) & 1;
  // process the character
  if (zero > 30 && c == 1 && previous == 0){
    //Send(msg, message);
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
      // Check if there are more zero or one values
      if (zero > one){
	if(totalBits != 0){
	  //printf("0");
	  message = message << 1;
	}
      }
      else{
	if(totalBits != 0){
	  message = ((message << 1)|0x01);
	  //printf("1");
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
    //Send(msg, message);
    //printf(" %x ", message);
    //printf(" %c \n", message);
    Send(msg, message);
    message = 0;
    zero = 0;
    one = 0;
    totalBits = 0;
  }
  previous = c;
  Finish();
  //}
  //stop = -1;
}

int main (void) {
  long long timer_data;
  gg_init(11);
  timer_data = GetUs() + 125;
  while (1){
    message_decoder();
    message_handler();
    while (GetUs() <= timer_data); // wait for balance of 125us period
    timer_data += 125;
  }
  return 0;
}
