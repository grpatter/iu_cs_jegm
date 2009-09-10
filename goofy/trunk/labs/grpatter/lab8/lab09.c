#include <bsp/ggbsp.h>
#include "channel.h"
#include "gg_music.h"

int msg = -1;
int stop = 0;

void play_sound(){
	int i;
	for (i = 0; i < sizeof(sound_0); i++)
	{
		DACR = sound_0[i] << 8; // must be aligned on byte 2
		delay_125uS(2);
	}
}

void mv_motors(int mode){
	IO0DIR |= 1<<21;//set pin DIRs
	IO0DIR |= 1<<20;
	IO0DIR |= 1<<0;
	IO1DIR |= 1<<19;

	IO0CLR = 1<<21;//make sure all CLR
	IO0CLR = 1<<20;
	IO0CLR = 1<<0;
	IO1CLR = 1<<19;

	if(mode == 1){
		IO0SET = 1<<21;//set in FORWARD motion
		IO0SET = 1<<20;
		IO0SET = 1<<0;
		IO1SET = 1<<19;
	}else if(mode == 2){  
		IO0SET = 1<<21;//set in BACKWARD motion
		IO0SET = 1<<20;
		IO0SET = 1<<0;
		IO1SET = 1<<19;
		IO0CLR = 1<<20;
		IO1CLR = 1 <<19; 
	}else if(mode == 3){
		IO0SET = 1<<21;//set to ROTATE
		IO0SET = 1<<20;
		IO0SET = 1<<0;
		IO1SET = 1<<19;
		IO1CLR = 1<<19;
	}//otherwise do nothing
	
	IO0CLR = 1<<21;//CLR before exit
	IO0CLR = 1<<20;
	IO0CLR = 1<<0;
	IO1CLR = 1<<19;
}


int message_handler()
{
	static int data;
	Begin();
	while (1){
		Receive(msg, data);
		//putchar(data);
		if(data == 0xA1) {
			printf("Forward\n");
			mv_motors(1);
		}
		if(data == 0xB1){
			printf("Music\n");
			play_sound();
		}
		if(data == 0xC1){
			printf("Rotating...\n");
			mv_motors(3);
		}
		if(data == 0xD1){
			printf("Released\n");
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
	gg_init(9);
	timer_data = GetUs() + 125;
	while (1){
		message_decoder();
		message_handler();
		while (GetUs() <= timer_data); // wait for balance of 125us period
		timer_data += 125;
	}
	return 0;
}
