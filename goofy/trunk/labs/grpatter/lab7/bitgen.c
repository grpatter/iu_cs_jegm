#include <stdlib.h>
#include <stdio.h>

void putbit(char c, int iter)
{
  //  fprintf(stderr, "bitgen putting %c %d\n", c, iter);
  while (iter)
    {
      putchar(c);
      iter--;
    }
}

void putsymbol(unsigned int data)
{
  int i;
  int idle;
  int start;

  start = 1 + random();
  if (start > 126)      // 0xff looks like EOF hence limit below 127
    start = 126;
  idle = 40 + (random() & 7);

  putbit('1', start);
  putbit ('0',idle);

  for (i = 7; i >= 0; i--)
    {
      int high;
      int low;
      int period;
      period = 12 + (random() & 7);
      if (data & (1<<i))
	{
	  high = period * 0.6;
	  low = period * 0.4;
	}
      else
	{
	  high = period * 0.4;
	  low = period * 0.6;
	}
      putbit('1', high);
      putbit('0', low);
    }
}

int main()
{
  int i;
  int c;
  while ((c = getchar()) != EOF)
    {
      putsymbol(c);
    }
}
