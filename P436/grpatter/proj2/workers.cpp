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
void reportNewWork(Work cur_job);
void reportWorkDone(Work cur_job, int exec_time);
void *executeWork(void *id);
void *createWork(void *id);
bool hasDupeResource(int index, int x, int rec[]);
void lockResources(Work cur_job, int num);
void unlockResources(Work cur_job, int num);

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

  printf("------------------------\n");
  printf("Workers:\t%d\n", worker);
  printf("Starting Work:\t%d\n", start_work);
  printf("Work Arrival:\t%d\n", work_arr_t);
  printf("Resources:\t%d\n", resources);
  printf("Work Resources:\t%d\n", req_workres);
  printf("Work Time:\t%d\n", work_t_max);
  printf("Queue Size:\t%d\n", q_size);
  printf("------------------------\n\n");
  
  srand(time(NULL));//DONT SEED in the function (same pseudo sequence)
  for(int a = 0; a < start_work; a++){
	createWork((void *)true);
  }
  printf("Added Work items before thread creation, IDs: 0-%d\n", start_work);
  
  initThreads();
  while(1);  
}

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
		reportWorkDone(cur_job, difftime(time(&end),start));
	}else{
		sleep(1);
	}
  } while (true);
}

void lockResources(Work cur_job, int num) {
  for(int i = 0; i < num; i++) {
    pthread_mutex_lock(&resource_locks[cur_job.resources[i]]);
	sleep(cur_job.time);
  }
}

void unlockResources(Work cur_job, int num){
  for(int i = 0; i < num; i++) {
    pthread_mutex_unlock(&resource_locks[cur_job.resources[i]]);
  }
}

void *createWork(void *i) {
  bool initial_build = (bool)i;  
  do {
	if(!initial_build){
		sleep( rand() % work_arr_t + 1);
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
    
    int i = 0;
    while(i < resource_count) {
      if (!hasDupeResource(i, rand_res_requests[i], rand_res_requests)){
		i++;
      }else {
		rand_res_requests[i] = rand() % resources;
      }
    }
    
    for (int i = 0; i < resource_count; i++) {
      new_work.resources[i] = rand_res_requests[i];
    }
    
	if(!initial_build){
		reportNewWork(new_work);
	}
    work.push(new_work);
  } while (true && !initial_build);
}

bool hasDupeResource(int index, int x, int rec[]) {
  bool result = false;
  for(int i = 0; i < sizeof(rec); i++) {
    if(rec[i] == x && i != index) {
      return true;      
    }
  }
  return result;
}

void reportNewWork(Work cur_job) {
  pthread_mutex_lock(&output_lock);
  printf("Added ID:      %d Time: %d Resources: ", cur_job.id, cur_job.time);
  for(int i = 0; i < cur_job.resource_count; i++) {
    printf("%d ", cur_job.resources[i]);
  }
  printf("\n");
  pthread_mutex_unlock(&output_lock);
}

void reportWorkDone(Work cur_job, int exec_time) {
   pthread_mutex_lock(&output_lock);
  printf("Completed ID:  %d Time: %d Actual Time: %d Resources: ", cur_job.id, cur_job.time, exec_time);
  for(int i = 0; i < cur_job.resource_count; i++) {
    printf("%d ", cur_job.resources[i]);
  }
  printf("\n");
  pthread_mutex_unlock(&output_lock);
}
