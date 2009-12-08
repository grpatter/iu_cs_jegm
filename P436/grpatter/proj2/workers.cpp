#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <time.h>

#define WORKER 10
#define STARTING_WORK 50
#define WORK_ARRIVAL 5
#define RESOURCES 20
#define WORK_RESOURCES 6
#define WORK_TIME 10
#define QUEUE_SIZE 1000

using namespace std;

typedef struct {
  int id;
  int resource_count;
  int resources[1000];
  int time;
} Work;

int worker = WORKER;
int work_t_max = WORK_TIME;
int start_work = STARTING_WORK;
int q_size = QUEUE_SIZE;
int work_arr_t = WORK_ARRIVAL;
int resources = RESOURCES;
int req_workres = WORK_RESOURCES;
pthread_mutex_t output_lock;
pthread_mutex_t *resource_locks;
int cur_ind = 0;
queue<Work> work;

void initThreads();
void *executeWork(void *id);
void *createWork(void *id);
bool hasDupeResource(int index, int x, int rec[]);
void lockResources(Work cur_job, int num);
void unlockResources(Work cur_job, int num);
void reportHandler(Work cur_job, int exec_time);
vector<int> createSortVec(Work cur_job);

/********************************************//**
 *  Deadl with CL arguments. If one is not provided, it is defaulted.
 *  Then prints a summary of settings and starts the threads for further
 *  processing.
 ***********************************************/
int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-worker") == 0) {
      worker = atoi(argv[i+1]);
    }else if (strcmp(argv[i], "-starting-work") == 0) {
      start_work = atoi(argv[i+1]);
    }else if (strcmp(argv[i], "-work-arrival") == 0) {
      work_arr_t = atoi(argv[i+1]);
    }else if (strcmp(argv[i], "-resources") == 0) {
      resources = atoi(argv[i+1]);
    }else if (strcmp(argv[i], "-work-resources") == 0) {
      req_workres = atoi(argv[i+1]);
    }else if (strcmp(argv[i], "-work-time") == 0) {
      work_t_max = atoi(argv[i+1]);
    }else if (strcmp(argv[i], "-queue-size") == 0) {
      q_size = atoi(argv[i+1]);
    }else {
      if(argc%2 == 0) {
		fprintf(stderr, "Invalid Arguments. Exiting.\n");
		exit(1);
      }
    }
  }
  
  srand(time(NULL));//DONT SEED in the function (same pseudo sequence)
  for(int a = 0; a < start_work; a++){
	createWork((void *)true);
  }

  printf("**\n");
  printf("Workers:\t%d\n", worker);
  printf("Starting Work:\t%d\n", start_work);
  printf("Work Arrival:\t%d\n", work_arr_t);
  printf("Resources:\t%d\n", resources);
  printf("Work Resources:\t%d\n", req_workres);
  printf("Work Time:\t%d\n", work_t_max);
  printf("Queue Size:\t%d\n", q_size);
  printf("**\n");
  printf("Added Work items before thread creation, IDs: 0-%d\n\n", start_work);
  
  initThreads();
  while(1);//let threads run forever
}

/********************************************//**
 *  Starts up all threads and their respective mutexes.
 *  Uses pthread_t, pthread_mutex, and pthread_create to do so.
 *  Kicks the threads into the appropriate functions.
 ***********************************************/
void initThreads(){
  pthread_mutex_init(&output_lock, NULL);
  
  pthread_t *worker_th;
  worker_th = new pthread_t[worker];
  pthread_t workadd_th;
  pthread_create(&workadd_th, NULL, createWork, (void *)false);
  
  resource_locks = new pthread_mutex_t[resources];
  for(int x = 0; x < worker; x++){
	pthread_mutex_init(&resource_locks[x], NULL);	
    pthread_create(&worker_th[x], NULL, executeWork, (void *)x);
  }
}

/********************************************//**
 *  Deals with 'job' execution. Attempts to pull a piece
 *  of work off of the queue, locks the necessary resources
 *  and complete the job. Then unlocks the resources and tries
 *  to find more work. If none is available, the thread will pause
 *  before checking again. 
 ***********************************************/
void *executeWork(void *id) {
  do {
	if(!work.empty()){
		Work cur_job = work.front();
		work.pop();
		int num = cur_job.resource_count;
		time_t start, end;
		time(&start);
		lockResources(cur_job, num);
		unlockResources(cur_job, num);
		reportHandler(cur_job, difftime(time(&end),start));
	}else{
		sleep(1);
	}
  } while (true);
}

/********************************************//**
 *  Deals with actually locking the resources a thread needs.
 *  Takes a job struct and a number (representing total resources needed)
 *  and attempts to lock them. If successful, it sleeps for the requested time
 *  and then moves to the next resource.
 ***********************************************/
void lockResources(Work cur_job, int num) {
	vector<int> v = createSortVec(cur_job);
  for(int i = 0; i < num; i++) {
    pthread_mutex_lock(&resource_locks[v[i]]);
	sleep(cur_job.time);
  }
}

/********************************************//**
 *  Takes a (Work)job and puts it's requested resources
 *  into a vector which is then sorted using the default
 *  library call. This sorted vector is then returned
 *  to the caller.
 ***********************************************/
vector<int> createSortVec(Work cur_job){
  vector<int> v (cur_job.resources, cur_job.resources + cur_job.resource_count);
  sort (v.begin(), v.end());
  return v;
}

/********************************************//**
 *  Deadls with unlocking the resources a thread has used.
 *  Takes a job struct and a number (representing total resources for job)
 *  and unlocks them so they can be used by other threads/jobs.
 ***********************************************/
void unlockResources(Work cur_job, int num){
  for(int i = 0; i < num; i++) {
    pthread_mutex_unlock(&resource_locks[cur_job.resources[i]]);
  }
}

/********************************************//**
 *  Deals with creating (and adding) more work for the queue.
 *  Deals with some initial build peculiarities, and ensures
 *  resource # need generation is randomized (and not duplicated).
 ***********************************************/
void *createWork(void *i) {
  bool initial_build = (bool)i;  
  do {
	if(work.size() < q_size){
		if(!initial_build){
			sleep(rand() % work_arr_t + 1);
		}
		int resource_count = rand() % req_workres + 1;
		int rand_res_requests[resource_count];		
		Work new_work;
		new_work.id = cur_ind++;
		new_work.resource_count = resource_count;
		new_work.time = rand() % work_t_max;
		
		for (int i = 0; i < resource_count; i++) {
		  rand_res_requests[i] = rand() % resources;
		}		
		int j = 0;
		while(j < resource_count) {
		  if (!hasDupeResource(j, rand_res_requests[j], rand_res_requests)){
			j++;
		  }else {
			rand_res_requests[j] = rand() % resources;
		  }
		}		
		for (int x = 0; x < resource_count; x++) {
		  new_work.resources[x] = rand_res_requests[x];
		}
		
		if(!initial_build){
			reportHandler(new_work, -1);
		}
		work.push(new_work);
	}else{
		sleep(1);//set to 1 for now, could be anything
	}
  } while (true && !initial_build);
}

/********************************************//**
 *  Takes two values and an array, loops through the array,
 *  and ensures value x is not at more than 1 location
 *  in the array.
 ***********************************************/
bool hasDupeResource(int index, int x, int rec[]) {
  bool res = false;
  for(int i = 0; i < sizeof(rec); i++) {
    if(rec[i] == x && i != index) {
      res = true;
		break;
    }
  }
  return res;
}

/********************************************//**
 *  Deals with printlocking and reporting
 *  to output (STDOUT in most cases) newly added
 *  and completed jobs.
 ***********************************************/
void reportHandler(Work cur_job, int exec_time){
	pthread_mutex_lock(&output_lock);
	if(exec_time > 0){//work done
	  printf("Completed ID:  %d Time: %d Actual Time: %d Resources: ", cur_job.id, cur_job.time, exec_time);
	  for(int i = 0; i < cur_job.resource_count; i++) {
		printf("%d ", cur_job.resources[i]);
	  }
	}else{//new work
	  printf("Added ID:      %d Time: %d Resources: ", cur_job.id, cur_job.time);
	  for(int i = 0; i < cur_job.resource_count; i++) {
		printf("%d ", cur_job.resources[i]);
	  }
	}
	printf("\n");
	pthread_mutex_unlock(&output_lock);
}

