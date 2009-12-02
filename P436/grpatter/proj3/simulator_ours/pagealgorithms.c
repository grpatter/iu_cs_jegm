#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <list>
#include "simulator.h"

//defines

//enums
enum AlgorithmType {FIFO, LRU, CLOCK, ECLOCK, RANDOM};

//structs
typedef struct{
	frame_t frame;
	int revs;
}clock_frame;

//function prototypes
void initAlgorithm(char *algo);
void initCurAlgo();
int getVictim(frame_t *victim);
void swapInActionAlgo(frame_t ff);
void clockVictim(frame_t *victim);
void lruAccessUpdate(frame_t ff);
void accessActionAlgo(frame_t ff);


int CUR_ALGO;
char *alg_string;
std::queue<frame_t> fifo_queue;
std::queue<frame_t> lru_queue;
std::list<clock_frame> clock_list;
int clock_hand_loc;

void initAlgorithm(char *algo){
	alg_string = algo;
	
	if(strcmp(algo, "fifo") == 0){
		stats.algorithm = "fifo";
		CUR_ALGO = FIFO;
	}else if(strcmp(algo, "lru") == 0){
		stats.algorithm = "lru";
		CUR_ALGO = LRU;
	}else if(strcmp(algo, "clock") == 0){
		stats.algorithm = "clock";
		CUR_ALGO = CLOCK;
	}else if(strcmp(algo, "eclock") == 0){
		stats.algorithm = "eclock";
		CUR_ALGO = ECLOCK;
	}else{
		CUR_ALGO = RANDOM;
		stats.algorithm = "Defaulted to random";
	}
	initCurAlgo();
	printf("CUR_ALGO num is: %d-----------------------!!!!!!!!-----------------\n",CUR_ALGO);
}

void initCurAlgo(){
	switch(CUR_ALGO){
		case FIFO:
			//no op
			break;
		case LRU:
			//no op
			break;
		case CLOCK:
			clock_hand_loc = -1;//so first add gets 0 index
			break;
		case ECLOCK:
			clock_hand_loc = -1;//so first add gets 0 index
			break;
		default:
			//no op, random
			break;
	}
}

void clockVictim(frame_t *victim){
int size = clock_list.size();
	//for(int i = 0; i < (2*size)+1; i++){
	while(1){//ugh...
		clock_frame cf = (clock_frame)clock_list.front();//get el
		clock_list.pop_front();//remove it
		if(clock_hand_loc > size){
			clock_hand_loc = 0;
		}
		if(cf.revs == 0){//great we can use this
			*victim = cf.frame;
			clock_hand_loc++;
			break;
		}else{//decrease rev and push back on
			cf.revs--;
			clock_list.push_back(cf);
			clock_hand_loc++;
		}
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
			clockVictim(victim);
			break;
		case ECLOCK:
			clockVictim(victim);
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
			clock_frame cf;
			cf.frame = ff;
			cf.revs = 1;
			clock_list.push_back(cf);
			break;
		case ECLOCK:
			clock_frame ecf;
			ecf.frame = ff;
			ecf.revs = 2;
			clock_list.push_back(ecf);
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
//printf("lru size:%d looking for frame:%d...\n",size, ff);
frame_t match;
bool matchFound = false;
for(int i = 0; i < size; i++){
	//printf("lru loop iter:%d...\n",i);
	frame_t temp = lru_queue.front();//get it
	lru_queue.pop();//advance
	if(temp == ff){
		//printf("lru access found match, holding to end...Frame:%d\n", temp);
		match = temp;
		matchFound = true;
	}else{
		//printf("lru access no match, pushing back on...Frame:%d\n",temp);
		lru_queue.push(temp);
	}
}
if(matchFound){
	lru_queue.push(match);
}
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
