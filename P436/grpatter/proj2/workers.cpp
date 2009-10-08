#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <queue>

#define WORKER 10
#define STARTING_WORK 50
#define WORK_ARRIVAL 5
#define RESOURCES 20
#define WORK_RESOURCES 20
#define WORK_TIME 10
#define QUEUE_SIZE 1000

using namespace std;

// Work struct
typedef struct {
  int id;
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
queue<Work> work;

// Function Declerations
void printAddedWork(int requiredResources[], int uniqueID, int timeToFinish);
void *executeWork(void *tID);
void *addWork(void *tID);
bool unduplicate(int index, int x, int tempRec[]);

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

  // Initialize Mutexs
  pthread_mutex_init(&printLock, NULL);

  // Worker Threads
  pthread_t *workerThread;
  workerThread = new pthread_t[worker];
  // Delivery Thread
  pthread_t deliveryThread;
  pthread_create(&deliveryThread, NULL, addWork, (void *)NULL);
  // Resources
  pthread_mutex_t *resourceLock;
  resourceLock = new pthread_mutex_t[resources];

  for(int x = 0; x < worker; x++){
	pthread_mutex_init(&resourceLock[x], NULL);	
    pthread_create(&workerThread[x], NULL, executeWork, (void *)x);
  }
  while(1);
  
}

void *executeWork(void *tID) {
  do {
	printf("Thread[%d] picking up work.\n",(int)tID);
	if(!work.empty()){
		Work thisJob = work.front();
		work.pop();
		int arraySize = sizeof(thisJob.resources)/sizeof(thisJob.resources[0]);
		//lockResources();
		//sleepForWork();
		//print();
		//unlockResources();
	}else{
		sleep(1);
	}
  } while (true);
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
  } while (true);
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


void printAddedWork(int requiredResources[], int uniqueID, int timeToFinish) {
  pthread_mutex_lock(&printLock);
  printf("Added ID:      %d Time: %d Resources: ", uniqueID, timeToFinish);
  for  (int i = 0; i < sizeof(requiredResources); i++) {
    printf("%d ", requiredResources[i]);
  }
  pthread_mutex_unlock(&printLock);
}
