#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#define PHILCOUNT 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

using namespace std;
int state[PHILCOUNT];
int state_counts[PHILCOUNT][2];
pthread_mutex_t chopsticks[PHILCOUNT];
pthread_mutex_t output_lock;//init
int num_users;
bool time_out = false;

int main(int argc, char **argv);
void pickChopsticks(int id);
void dropChopsticks(int id);
void eat(int id);
void think(int id);
void *doWork(void *tID);
void ouputInfo(char *str1, int i, char *str2);

//threads default
void *doWork(void *tID){
  int id = (int) tID;
  do {
    think(id);
    pickChopsticks(id);
    eat(id);
    dropChopsticks(id);
  } while (!time_out);
}

void pickChopsticks(int id){
  if(id%2 == 0){
    pthread_mutex_lock(&chopsticks[id]);
    pthread_mutex_lock(&chopsticks[(id + 1) % num_users]);
  } else {
    pthread_mutex_lock(&chopsticks[(id - 1) % num_users]);
    pthread_mutex_lock(&chopsticks[id]);
  }
}

void dropChopsticks(int id){
  if(id%2 == 0){
    pthread_mutex_unlock(&chopsticks[id]);
    pthread_mutex_unlock(&chopsticks[(id + 1) % num_users]);
  } else {
    pthread_mutex_unlock(&chopsticks[(id - 1) % num_users]);
    pthread_mutex_unlock(&chopsticks[id]);
  }
}

void think(int id){
  state[id] = THINKING;
  state_counts[id][1]++;
  ouputInfo("Philosopher ", id, ": \tTHINKING!");
  srand( time(NULL) );
  int r_num = rand() % 5000 + 1;
  usleep( (useconds_t)r_num);
}

void eat(int id){
  state[id] = EATING;
  state_counts[id][0]++;
  ouputInfo("Philosopher ", id, ": EATING!");
  srand( time(NULL) );
  int r_num = rand() % 5000 + 1;
  usleep( (useconds_t)r_num);
}

void ouputInfo(char *str1, int i, char *str2){
  pthread_mutex_lock(&output_lock);
  printf("%s%d %s\n", str1, i, str2);
  pthread_mutex_unlock(&output_lock);
}

int main(int argc, char *argv[]) {
  pthread_t *threads;
  
  int run_time = atoi(argv[1]);
  if (argc == 3){
    threads = new pthread_t[atoi(argv[2])];
    num_users = atoi(argv[2]);
  }else{
    threads = new pthread_t[PHILCOUNT];
    num_users = PHILCOUNT;
  }

  printf("Time To Live (seconds): %d\n", run_time);
  printf("Number Dining Philosophers: %d\n", num_users);
  printf("----------------------------------------\n");
  
  pthread_mutex_init(&output_lock, NULL);
  for(int i = 0; i < num_users; i++){
    pthread_mutex_init(&chopsticks[i], NULL);
  }
  for(int i = 0; i < num_users; i++){
    pthread_create(&threads[i], NULL, doWork, (void *)i);
  }
  
  sleep(run_time);
  time_out = true;
  
  sleep(1);//let threads exit
  printf("----------------------------------------\n");
  for(int i = 0; i < num_users; i++) {
    printf("Philosopher %d: Ate %d / Thought %d\n", i, state_counts[i][0], state_counts[i][1]);
  }
  printf("----------------------------------------\n");
  
  return 0;
}
