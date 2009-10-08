#include <stdio.h>
#include <pthread.h>

#define NUM_DINERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT ((i-1)%NUM_DINERS)
#define RIGHT ((i+1)%NUM_DINERS)
void think();
void eat();
void test(int i);
void release_chopsticks(int i);
void take_chopsticks(int i);
void philosopher(int i);

int state[NUM_DINERS]; //initialize all states to THINKING
typedef int semaphore;
semaphore mutex = 1;//mutual exclusion
semaphore s[NUM_DINERS];//one semephore per phil

void philosopher(int i){//i is phil num, 0 ... n-1
	while(1){
		think();//think state
		take_chopsticks(i);//acquire or block
		eat();//eating
		release_chopsticks(i);//unblock
	}
}

void take_chopsticks(int i){
	down(&mutex);//wait(&mutex);//enter 'critical' region
	state[i] = HUNGRY;//record state
	test(i);//try 
	up(&mutex);//signal(&mutex);//exit 'critical' region
	down(&s[i]);//wait(&s[i]);//block if didnt pass test
}

void release_chopsticks(int i){
	down(&mutex);//wait(&mutex);//enter crit
	state[i] = THINKING;//done eating change state
	test (LEFT);//see if neighbor can eat 
	test (RIGHT);//see if neighbor can eat
	up(&mutex);//signal(&mutex);//exit critical region
}

void test(int i){
	if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
		state[i] = EATING;
		up(&s[i]);//signal(&s[i]);
	}
}
		
void think(){
	sleep(1);
}
void eat(){
	sleep(2);
}
			
