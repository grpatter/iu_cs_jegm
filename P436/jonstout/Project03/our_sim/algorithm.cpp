/*
** Jonathan M. Stout
*/

#include <iostream>
#include <queue>
#include <string.h>
#include <stdbool.h>
#include "algorithm.h"
#include "simulator.h"

using namespace std;

// Algorithm we're using
// 0 = fifo | 1 = lru | 2 = clock | 3 = eclock | 4 = random 
int algoIndex;

queue<frame_t> frameQueue;
list<cFrame> clockList;
int clockPointer;

// Sets clockPointer = -1 so that after the first
// add the index will be at zero
void init() {
	if (algoIndex == 2 || algoIndex == 3)
		clockIndex = -1;
}

// When called will set algoIndex so we know
// for future fuctions what should be done
// accourding to type
void setAlgorithm(char* algo) {
	if (strcmp(algo, "fifo") == 0) {
		algoIndex = 0;
	}
	else if (strcmp(algo, "lru") == 0) {
		algoIndex = 1;
	}
	else if (strcmp(algo, "clock") == 0) {
		algoIndex = 2;
	}
	else if (strcmp(algo, "eclock") == 0) {
		algoIndex = 3;
	}
	else {
		algoIndex = 4;
	}
	init();
}

// Gets vic with reference bit not set while
// at the same time keeping track of where
// the oldest frame is.
int getClockVictim(frame_t *victim) {
	int size = clockList.size();
	int done = 0;
	while(1) {
		if(clockPointer > size)
			clockPointer = 0;
		if(clockList.front().ref == -1) {
			*victim = clockList.front();
			clockList.pop_front();
			clockPointer++;
			break;
		}
		else {
			clockList.front().ref = -1;
			clockList.push_back( clockList.pop_front() );
			clockPointer++;
		}
	}
}

// Gets vic of fifo and lru sends to getClockVictim
// for second-chance and Enhanced second-chance
int getVictim(frame_t* victim) {
	if (algoIndex == 0 || algoIndex == 1) {
		*victim =  frameQueue.front();
		frameQueue.pop();
		return 0;
	}
	else if (algoIndex == 2 || algoIndex == 3) {
		getClockVictim(victim);
		return 0;
	}
	else {
		return -1;
	}
}

// adds frame to queue or list based on algoIndex
void addFrame(frame_t frame) {
	if (algoIndex == 0 || algoIndex == 1) {
		frameQueue.push(frame);
	}
	else if (algoIndex == 2 || algoIndex == 3) {
		cFrame temp;
		temp.ref = 0;
		temp.frame = frame;
		clockList.pushBack(temp);
	}
	else {
		// DO NOTHING
	}
}

void updateFrames(frame_t frame) {
	// Finds frame and adds to end of queue
	if (algoIndex == 1) {
		frame_t result;
		bool match = false;
		for (int i = 0; i < frameQueue.size(); i++) {
			frame_t temp = frameQueue.pop();
			if (frame == temp) {
				result = temp;
				match = true;
			}
			else {
				frameQueue.push(temp);
			}
		}
		if (match)
			frameQueue.push(result);
	}
	else {
		// DO NOTHING
	}
}