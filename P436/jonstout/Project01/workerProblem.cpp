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

// Function Declerations
void printAddedWork(int requiredResources[], int uniqueID, int timeToFinish);

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
  // Delevery Thread
  pthread_t deleveryThread;
  // Queue
  queue<Work> work;
  // Resources
  pthread_mutex_t *resourceLock;
  resourceLock = new pthread_mutex_t[resources];

  
}

void *exicuteWork(void *tID) {
  do {
    //lockResources();
    //sleepForWork();
    //print();
    //unlockResources();
  } while (true);
}

void *addWork(void *tID) {
  do {
    srand(time(NULL));
    int numberOfResources = rand() % workResources;
    int i;

    Work moreWork;
    moreWork.id = nextID++;
    moreWork.time = rand() % workTime;

    for (i = 0; i < numberOfResource; ++i){
      

    }

  } while (true);
}

void printAddedWork(int requiredResources[], int uniqueID, int timeToFinish) {
  pthread_mutex_lock(&printLock);
  printf("Added ID:      %d Time: %d Resources: ", uniqueID, timeToFinish);
  for  (int i = 0; i < sizeof(requiredResources); i++) {
    printf("%d ", requiredResources[i]);
  }
  pthread_mutex_unlock(&printLock);
}
