#include "goofylib.h"
int main (void) {
  gg_init(12);
  timer1_setup();
  vic_setup();
  while (1) {
    message_handler();
  }
}

