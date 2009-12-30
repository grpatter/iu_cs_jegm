/*
** Jonathan M. Stout
*/
#include <stdio.h>
#include "simulator.h"

// Structs // ----------------
typedef struct {
	frame_t frame;
	int ref;
} cFrame;

// Functions // --------------
void setAlgorithm(char* algo);
int getVictim(frame_t frame);
void addFrame(frame_t frame);
void updateFrames(frame_t frame);
