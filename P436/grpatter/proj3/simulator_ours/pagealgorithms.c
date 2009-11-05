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


int CUR_ALGO;
char *alg_string;
std::queue<frame_t> fifo_queue;
std::queue<frame_t> lru_queue;

void initAlgorithm(char *algo){
	alg_string = algo;
	if(strcmp(algo, "fifo") == 0){
		CUR_ALGO = FIFO;
	}else if(strcmp(algo, "lru") == 0){
		CUR_ALGO = LRU;
	}else if(strcmp(algo, "clock") == 0){
		CUR_ALGO = CLOCK;
	}else if(strcmp(algo, "eclock") == 0){
		CUR_ALGO = ECLOCK;
	}else{
		CUR_ALGO = RANDOM;
	}
	initAlgo();
}

void initAlgo(){
	switch(CUR_ALGO){
		case FIFO:
			//no op
			break;
		case LRU:
			//no op
			break;
		case CLOCK:
			//no op
			break;
		case ECLOCK:
			//no op
			break;
		default:
			//no op, random
			break;
	}
}

int getVictim(frame_t *victim){
	switch(CUR_ALGO){
		case FIFO:
			*victim = fifo_queue.front();//get victim
			fifo_queue.pop();//move queue forward
			break;
		case LRU:
			*victim = lru_queue.front();//get victim
			lru_queue.pop();//move queue forward
			break;
		case CLOCK:
			//no op
			break;
		case ECLOCK:
			//no op
			break;
		default:
			return -1; //invalid, use default
			break;
	}
	return 0;
}

void swapInActionAlgo(frame_t ff){
	switch(CUR_ALGO){
		case FIFO:
			fifo_queue.push(ff);//add to queue
			break;
		case LRU:
			lru_queue.push(ff);//add to queue
			break;
		case CLOCK:
			//no op
			break;
		case ECLOCK:
			//no op
			break;
		default:
			//no op, random
			break;
	}
}

void lruAccessUpdate(frame_t ff){
//find given frame
//pop off then push on back
int size = lru_queue.size();
frame_t match;
for(int i = 0; i < size; i++){
	frame_t temp = lru_queue.front();//get it
	lru_queue.pop();//advance
	if(temp == ff){
		match = temp;
	}else{
		lru_queue.push(temp);
	}
}
lru_queue.push(match);
}

void accessActionAlgo(frame_t ff){
	switch(CUR_ALGO){
		case FIFO:
			break;
		case LRU:
			lruAccessUpdate(ff);
			break;
		case CLOCK:
			//no op
			break;
		case ECLOCK:
			//no op
			break;
		default:
			//no op, random
			break;
	}
}
