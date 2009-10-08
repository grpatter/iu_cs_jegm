#include <stdio.h>
#include "channel.h"

int msg = -1;
int stop = 0;

int message_handler()
{
  static int data;
  Begin();
  while (1)
    {
      Receive(msg, data);
      putchar(data);
    }
}

int message_decoder()
{
  static unsigned int message = 0;
  static int zero = 0;
  static int one = 0;
  static int totalBits = 0;

  static char previous = '1';
  static char c;

  Begin();
  while ((c = getchar()) != EOF)
    {
      // process the character
if (zero > 30 && c == '1' && previous == '0'){
	//Send(msg, message);
	message = 0;
	zero = 0;
	one = 1;
	totalBits = 0;
      }
      if ( c == '0' ){
	zero ++;
      }
      if (c == '1'){
	if(previous == '0'){
	  // Check if there are more zero or one values
	  if (zero > one){
	    if(totalBits != 0){
	      printf("0");
	      message = message << 1;
	    }
	  }
	  else{
	    if(totalBits != 0){
		message = ((message << 1)|0x01);
		printf("1");
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
	printf(" %x ", message);
	printf(" %c \n", message);
	message = 0;
	zero = 0;
	one = 0;
	totalBits = 0;
      }
	previous = c;
    }
  stop = -1;
}

int main()
{
  
  while (!stop)
    {
      message_decoder();
      message_handler();
    }
  return 0;
}
