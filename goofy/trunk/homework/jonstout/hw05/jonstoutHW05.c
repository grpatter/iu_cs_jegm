#include <stdio.h>

unsigned int switchBits();
void mnCode(unsigned int eOne);
void regname(unsigned int eOne);
void iregname(unsigned int eOne);
unsigned int twentyth(unsigned int eOne);

int main()
{
  unsigned int eOne;
  int count;
  unsigned int twoFive;

  eOne = switchBits();
  twoFive = twentyth(eOne);
  
  count = 1;
  printf("%d ", count);
  printf("%X\t", eOne);

  if(twoFive == 0){        
    mnCode(eOne);
    regname(eOne);    
    printf("\n");
  }
  else{
    // TODO
    mnCode(eOne);
    iregname(eOne);
    printf("\n");
  }
}

unsigned int twentyth(unsigned int eOne)
{
  unsigned int result;

  result = (eOne >> 25) & 0xf;
}

unsigned int switchBits()
{
  unsigned int eOne;

  printf("Please input a 32bit hex number to parse.\n");
  scanf("%x", &eOne);
  
  eOne = (eOne >> 24)  |
    ((eOne<<8) & 0x00ff0000) |
    ((eOne>>8) & 0x0000ff00) |
    ((eOne<<24) & 0xff000000);
  
  return eOne;
}

void mnCode(unsigned int eOne)
{
  unsigned int mnem;
  char *mnemonic[] = {"AND","EOR","SUB","RSB","ADD","ADC","SBC","RSC","TST",
		      "TEQ","CMP","CMN","ORR","MOV","BIC","MVN"};

  mnem = (eOne >> 21) &0xf;
  printf("%s ", mnemonic[mnem]);
}

void regname(unsigned int eOne)
{
  unsigned int reg;
  char *regnames[] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10",
		      "r11","r12","sp","lr","pc"};

  reg = (eOne >> 12) &0xf;
  printf("%s, ", regnames[reg]);

  reg = (eOne >> 16) &0x0f;
  printf("%s, ", regnames[reg]);
  
  reg = (eOne >> 0) &0x0f;
  printf("%s ", regnames[reg]);
}

void iregname(unsigned int eOne)
{
  unsigned int imm;
  unsigned int shift;
  unsigned int reg;
  char *regnames[] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10",
		      "r11","r12","sp","lr","pc"};
  
  imm = (eOne >> 0) &0xff;
  shift = (eOne >> 8) &0xf;
  
  //eOne =  (shift*2) >> eOne;
  eOne =  eOne >> (shift*2);
  
  reg = (eOne >> 12) &0xf;
  printf("%s, ", regnames[reg]);
    
  reg = (eOne >> 16) &0x0f;
  printf("%s, ", regnames[reg]);
  
  // reg = (eOne >> 0) &0x0f;
  // printf("%s ", regnames[reg]);

  printf("#0x%x", imm);
}
