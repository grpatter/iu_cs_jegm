#include <bsp/ggbsp.h>
#include <stdio.h>

extern void ext0_ishell();

int vic_setup (void);
void null_handler(void) __attribute__ ((interrupt ("IRQ")));
void ext0_handler(void) __attribute__ ((interrupt ("IRQ")));

int main (void) {
  gg_init(3);  

  // Setup the external interrupt for pin P0.16 - This is integrated into ggbsp
  // but is currently commented out
  PINSEL1|=0x00000001;  // select EXT0 function on P0.16
  EXTMODE = 0x01;       // EXT0 Edge Triggered 
  EXTPOLAR = 0x01;      // EXT0 Rising Edge
  EXTINT=0x01;          // Write a one to clear any pending interrupt

  vic_setup();          // init the LPC VIC using newlib-lpc calls
  while (1);
  return 0;    
}

int vic_setup (void) {
  int error;
  int ret_val=0;
  if (VICInit(&null_handler)) {
    ret_val=-1;
  }
  if ((error=VICSetup(EINT0,MIN_VIC_PRIORITY-1,&ext0_handler,0))) {
    ret_val=-1;
  }
  return(ret_val);
}

void null_handler(void) {
  VICVectAddrRead=0x00;    // Causes an update of the VIC priority hardware
}

void ext0_handler(void) {
  // static int toggle=0;
  EXTINT=0x01;             // Write a one to clear any pending interrupt

  VICIntEnClr = (1 << 14);
  if (!(EXTPOLAR^=1)) {
    DS1_ON;
  }
  else {
    DS1_OFF;
  }
  
  VICIntEnable = (1 << 14);
  VICVectAddrRead=0x00;    // Causes an update of the VIC priority hardware
}


