#include <bsp/ggbsp.h>
#include <stdio.h>
#include "gg_music.h"

int  main(){
	gg_init(6);
	delay_125uS(2000);
	int i;
	for (i = 0; i < sizeof(sound_0); i++)
	{
	DACR = sound_0[i] << 8; // must be aligned on byte 2
	delay_125uS(2);
	}
	return 0;
}
