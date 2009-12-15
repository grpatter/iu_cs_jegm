/*
 * Greg Patterson
 * P436: Introducation to Operating Systems
 * Assignment 3: Memory Management Simulator
 *
 * This header file contains data structures and types necessary for the
 * simulator. While completing the assignment you may need to slightly alter
 * these data structures to complete the assignment.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include "simulator.h"

//defines

//enums
enum AlgorithmType {FIFO, LRU, CLOCK, ECLOCK, RANDOM};

//structs

//function prototypes
void initAlgorithm(char *algo);
void initAlgo();
int getVictim(frame_t *victim);
void swapInActionAlgo(frame_t ff);

