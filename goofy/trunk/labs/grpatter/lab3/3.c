#include <stdio.h>

typedef union
{
  unsigned char unchar[8];
  short snshort[4];
  int snint[2];
  float snfloat[2];
  double sndouble;
} poly;

void printDump(void *rawdata, int datasize)
{
  int i;
  int j;

  printf("Address\t    Char  HexCh Short   Integer\t        Float\t        DoubleFloat\n");

  poly *regData = (poly *) rawdata;

  for (i=0; i<8; i++){
    for( j = 0; j < sizeof(poly); j++){
      printf("0x%x   ", &regData[i].unchar[j]);
      printf("%c\t  ", regData[i].unchar[j]);
      printf("0x%x\t", regData[i].unchar[j]);

      if((j % 2) == 0)
	printf("%d\t", regData[i].snshort[j]);
      if((j % 4) == 0)
	printf("%d\t%e\t", regData[i].snint[j], regData[i].snfloat[j]);
      if((j % 8) == 0)
	printf("%e\t", regData[i].sndouble);
      printf("\n");	       
    }
  }
}

int main()
{
  int i;
  poly data[6];
  
  for(i=0; i<8; i++) 
    data[0].unchar[i] = 'a' + i;
  for(i=0; i<8; i++) 
    data[1].unchar[i] =  42 + i;
  for(i=0; i<4; i++) 
    data[2].snshort[i] = 5280 + i;
  for(i=0; i<2; i++)
    data[3].snint[i] = 'A' + i;
  
  printDump((void *) data, sizeof(data));
}
