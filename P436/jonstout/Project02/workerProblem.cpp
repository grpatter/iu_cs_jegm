#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <queue>
#include <vector>
#include <algorithm>

#define WORKER 10
#define STARTING_WORK 50
#define WORK_ARRIVAL 5
#define RESOURCES 20
#define WORK_RESOURCES 6
#define WORK_TIME 10
#define QUEUE_SIZE 1000

using namespace std;

// Work struct
typedef struct {
  int id;
  int numberOfResources;
  int resources[1000];
  int time;
} Work;

// Global Variables
int worker = WORKER;
int startingWork = STARTING_WORK;
int workArrival = WORK_ARRIVAL;
int resources = RESOURCES;
int workResources = WORK_RESOURCES;
int workTime = WORK_TIME;
int queueSize = QUEUE_SIZE;
pthread_mutex_t printLock;
int nextID = 0;
pthread_mutex_t *resourceLock;


// Queue
queue<Work> work;

// Function Declerations
void printAddedWork(int j, int requiredResources[], int uniqueID, int timeToFinish);
void *addWork(void *i);
bool unduplicate(int index, int x, int tempRec[]);
void *executeWork(void *i);
void lockResources(Work thisJob);
void printCompletedWork(int j, int requiredResources[], int uniqueID, int timeToFinish, double ActualTime);
void unlockResources(Work thisJob);
void printAddedWork(int j, int requiredResources[], int uniqueID, int timeToFinish);

int main(int argc, char *argv[]) {

  // Check for user arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-worker") == 0) {
      worker = atoi(argv[i+1]);
    }
    else if (strcmp(argv[i], "-starting-work") == 0) {
      startingWork = atoi(argv[i+1]);
    }
    else if (strcmp(argv[i], "-work-arrival") == 0) {
      workArrival = atoi(argv[i+1]);
    }
    else if (strcmp(argv[i], "-resources") == 0) {
      resources = atoi(argv[i+1]);
    }
    else if (strcmp(argv[i], "-work-resources") == 0) {
      workResources = atoi(argv[i+1]);
    }
    else if (strcmp(argv[i], "-work-time") == 0) {
      workTime = atoi(argv[i+1]);
    }
    else if (strcmp(argv[i], "-queue-size") == 0) {
      queueSize = atoi(argv[i+1]);
    }
    // Check for Invalid Arguments
    else {
      if(argc%2 == 0) {
	fprintf(stderr, "Invalid Arguments.\n");
	exit(1);
      }
    }
  }

  // Print arguments
  printf("\nWorker Problem Arguments\n");
  printf("------------------------\n");
  printf("Worker         : %d\n", worker);
  printf("Staring Work   : %d\n", startingWork);
  printf("Work Arrival   : %d\n", workArrival);
  printf("Resources      : %d\n", resources);
  printf("Work Resources : %d\n", workResources);
  printf("Work Time      : %d\n", workTime);
  printf("Queue Size     : %d\n", queueSize);
  printf("------------------------\n\n");

  // Worker Threads
  pthread_t *workerThread;
  workerThread = new pthread_t[worker];
  // Delevery Thread
  pthread_t deleveryThread;
  // Resources
  resourceLock = new pthread_mutex_t[resources];

  // Initialize Mutexs
  pthread_mutex_init(&printLock, NULL);
  for (int i = 0; i < resources; i++) {
    pthread_mutex_init(&resourceLock[i], NULL);
  }

  // Initialize Queue
  for (int i = 0; i < startingWork; i++) {
    int numberOfResources = rand() % workResources + 1;
    int newResources[numberOfResources];
    
    Work moreWork;
    moreWork.id = nextID++;
    moreWork.time = rand() % workTime;
    moreWork.numberOfResources = numberOfResources;
    // Array newResources is given resource numbers
    for (int i = 0; i < numberOfResources; i++) {
      newResources[i] = rand() % resources;
    }
    
    int i = 0;
    while(i < numberOfResources) {
      if (!unduplicate(i, newResources[i], newResources)){
	i++;
      }
      else {
	newResources[i] = rand() % resources;
      }
    }
    
    for (int i = 0; i < numberOfResources; i++) {
      moreWork.resources[i] = newResources[i];
    }
    work.push(moreWork);
  }

  // Do Work Thread
  for (int i = 0; i < worker; i++) {
    pthread_create(&workerThread[i], NULL, executeWork, (void *)NULL);
  }
  // Do Delevery Thread
  pthread_create(&deleveryThread, NULL, addWork, (void *)NULL);
 
  // Never exit main();
  while(1){
  }
}

void *executeWork(void *i) {
  do {
    while (work.empty()){
      sleep(1);
    }
    time_t start;
    time(&start);
    Work thisJob = work.front();
    work.pop();
    lockResources(thisJob);
    time_t end;
    time(&end);
    double dif = difftime(end, start);
    printCompletedWork(thisJob.numberOfResources, thisJob.resources, thisJob.id, thisJob.time, dif);
    unlockResources(thisJob);
  } while (true);
}

void lockResources(Work thisJob) {
  vector<int> myVect (thisJob.resources, thisJob.resources + thisJob.numberOfResources);
  sort (myVect.begin(), myVect.end());
  for (int i = 0; i < thisJob.numberOfResources; i++) {
    pthread_mutex_lock(&resourceLock[myVect[i]]);
  }
  sleep(thisJob.time);
}

void unlockResources(Work thisJob) {
 for (int i = 0; i < thisJob.numberOfResources; i++) {
    pthread_mutex_unlock(&resourceLock[thisJob.resources[i]]);
  }
}

void *addWork(void *i) {
  srand(time(NULL));
  
  do {
    sleep( rand() % workArrival + 1);

    int numberOfResources = rand() % workResources + 1;
    int newResources[numberOfResources];
    
    Work moreWork;
    moreWork.id = nextID++;
    moreWork.time = rand() % workTime;
    moreWork.numberOfResources = numberOfResources;
    // Array newResources is given resource numbers
    for (int i = 0; i < numberOfResources; i++) {
      newResources[i] = rand() % resources;
    }
    
    int i = 0;
    while(i < numberOfResources) {
      if (!unduplicate(i, newResources[i], newResources)){
	i++;
      }
      else {
	newResources[i] = rand() % resources;
      }
    }
    
    for (int i = 0; i < numberOfResources; i++) {
      moreWork.resources[i] = newResources[i];
    }
    
    printAddedWork(numberOfResources, moreWork.resources, moreWork.id, moreWork.time);
    work.push(moreWork);
  } while (true && work.size() < queueSize);
}

bool unduplicate(int index, int x, int tempRec[]) {
  bool result = false;
  for (int i = 0; i < sizeof(tempRec); i++) {
    if(tempRec[i] == x && i != index) {
      return true;      
    }
  }
  return result;
}

void printAddedWork(int j, int requiredResources[], int uniqueID, int timeToFinish) {
  pthread_mutex_lock(&printLock);
  printf("Added ID:      %d Time: %d Resources: ", uniqueID, timeToFinish);
  for  (int i = 0; i < j; i++) {
    printf("%d ", requiredResources[i]);
  }
  printf("\n");
  pthread_mutex_unlock(&printLock);
}

void printCompletedWork(int j, int requiredResources[], int uniqueID, int timeToFinish, double ActualTime) {
   pthread_mutex_lock(&printLock);
  printf("Completed ID:  %d Time: %d Resources: ", uniqueID, timeToFinish);
  for  (int i = 0; i < j; i++) {
    printf("%d ", requiredResources[i]);
  }
  // print ActualTime
  printf("Actual Time: %1.0f", ActualTime);
  printf("\n");
  pthread_mutex_unlock(&printLock);
}
