#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PHILO_COUNT 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

using namespace std;

// Global Variables
int state[PHILO_COUNT];
int stateCount[PHILO_COUNT][2];
pthread_mutex_t chopstick[PHILO_COUNT];
pthread_mutex_t printLock;// = PTHREAD_MUTEX_INITIALIZER;
int userCount;
bool end = false;

// Functions
int main(int argc, char **argv);
void think(int id);
void pickUpChops(int id);
void eat(int id);
void putDownChops(int id);
void *doYourJob(void *tID);
void myprint(char *chr, int i, char *chr2);

// Function for each philosopher (thread) to run
void *doYourJob(void *tID){
  int id = (int) tID;
  do {
    think(id);
    pickUpChops(id);
    eat(id);
    putDownChops(id);
  } while (!end);
}

int main(int argc, char *argv[]) {
  // Array of philosophers
  pthread_t *thread;
  
  // Grab arguments from Shell
  int userTime = atoi(argv[1]);
  if (argc == 3){
    thread = new pthread_t[atoi(argv[2])];
    userCount = atoi(argv[2]);
  }else{
    thread = new pthread_t[PHILO_COUNT];
    userCount = PHILO_COUNT;
  }

  // print out time and number of Philosophers
  printf("Time                  : %d\n", userTime);
  printf("Number of Philosophers: %d\n\n", userCount);
  
  //Initalize chopstick[]
  pthread_mutex_init(&printLock, NULL);
  for(int i = 0; i < userCount; i++){
    pthread_mutex_init(&chopstick[i], NULL);
  }
  
  //Create 5 threads that run doYourJob(void *tID)
  for(int i = 0; i < userCount; i++){
    pthread_create(&thread[i], NULL, doYourJob, (void *)i);
  }
  
  // Continue to run main until specified time has expired
  sleep(userTime);
  end = true;
  
  // Print out resulting data
  for(int i = 0; i < userCount; i++) {
    printf("Philosopher %d ate %d and thought %d\n", i, stateCount[i][0], stateCount[i][1]);
  }
  
  // Exit
  return 0;
}

/*
** Change current state to THINKING, increment stateCount,
** print out state, get a random number between 1 and 5000
** and sleep.
*/
void think(int id){
  state[id] = THINKING;
  stateCount[id][1]++;
  myprint("Philosopher: ", id, "THINKING");
  srand( time(NULL) );
  int ranNum = rand() % 5000 + 1;
  usleep( (useconds_t)ranNum);
}

/*
** Change current state to EATING, increment stateCount,
** print out state, get a random number between 1 and 5000
** and sleep.
*/
void eat(int id){
  state[id] = EATING;
  stateCount[id][0]++;
  myprint("Philosopher: ", id, "EATING");
  srand( time(NULL) );
  int ranNum = rand() % 5000 + 1;
  usleep( (useconds_t)ranNum);
}

// Lock mutexes based on odd philosopher or even philosopher
void pickUpChops(int id){
  if(id%2 == 0){
    pthread_mutex_lock(&chopstick[id]);
    pthread_mutex_lock(&chopstick[(id + 1) % userCount]);
  } else {
    pthread_mutex_lock(&chopstick[(id - 1) % userCount]);
    pthread_mutex_lock(&chopstick[id]);
  }
}

/*
** Unlock mutexes based on odd philosopher or even philosopher
** without regaurd to order.
*/
void putDownChops(int id){
  if(id%2 == 0){
    pthread_mutex_unlock(&chopstick[id]);
    pthread_mutex_unlock(&chopstick[(id + 1) % userCount]);
  } else {
    pthread_mutex_unlock(&chopstick[(id - 1) % userCount]);
    pthread_mutex_unlock(&chopstick[id]);
  }
}

// Lock print function, print, unlock print function
void myprint(char *chr, int i, char *chr2){
  pthread_mutex_lock(&printLock);
  printf("%s%d %s\n", chr, i, chr2);
  pthread_mutex_unlock(&printLock);
}
