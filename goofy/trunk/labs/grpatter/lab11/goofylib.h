#include "goofylib.c"

void mv_dirs(void);
void mv_forward(void);
void mv_rotate(void);
void mv_stop(void);
int message_handler();
int message_decoder();
void timer1_handler(void) __attribute__ ((interrupt ("IRQ")));
void null_handler(void) __attribute__ ((interrupt ("IRQ")));
void vic_setup (void);
void null_handler(void);
void timer1_setup(void);
void play_sound();
void timer1_handler(void);
