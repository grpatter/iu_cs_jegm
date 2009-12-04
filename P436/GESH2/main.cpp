//Greg Patterson
//grpatter at indiana.edu
//Eric Spencer
//erspence at indiana.edu
//main for custom shell impl
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jobs.h"
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <termios.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <errno.h>

using namespace std;
typedef struct filename FILENAME;

#define MAX_LEN 512
#define MAX_FILES 16324
int countchar(const char[], char);
int runJob(jobStruct * curJob);
void flush_io();
int get_job_position(int job_n, vector<jobStore> &jobStore);
void p_summary();
void p_jobs();
void batch_m(int argc, char *fname);
void std_m();
int handle_job(char *input, bool bg);
void change_status(char *status);
void child(int argc, char *argv[10]);

int cmd_n = 1;//keep track of # cmds
//struct jobStore jobStore[50];
vector<jobStore> jobStore;
char * PATH = "[GE_SH2]$ ";
char *path;


void flush_io(){
	fflush(stdin);
	fflush(stdout);
}

int runJob(jobStruct * curJob, int pid){
	printf("Command: %s\n", curJob->cmd_path);
	change_status("Running");
	int loc = get_job_position(cmd_n, jobStore);
	jobStore[loc].realJob->cmd_path = curJob->cmd_path;
	if(execvp(curJob->cmd_path, curJob->argv)== -1){
		cout << "Command Not Found: "<<curJob->cmd_path << endl;
		change_status("Error");
		exit(0);//kill this process
	}
}

void handle_cd(char *in, jobStruct * curJob){
	int childstatus, status;
	char prev_dir[1024];
	getcwd(prev_dir, sizeof(prev_dir));
	printf("Command: %s\n", in);
	change_status("Running");
	int loc = get_job_position(cmd_n, jobStore);
	jobStore[loc].realJob->cmd_path = curJob->cmd_path;
	if(chdir(curJob->argv[1]) == -1){
		perror("Problem changing directory:");
		//chdir(prev_dir);//we need to do this?
	}
	//cout<<"handle_cd done..."<<endl;
	change_status("Completed");
}


int listDir(char *dir, bool show_long, bool show_all){
	char **dirs;
	DIR *dir_ptr;
	struct dirent *cur_dir;
	struct stat stat;
	FILENAME *temp_file;
	FILENAME **files;
	int file_count = 0;
	int dir_count = 0;
	char perms[16];
	files = (FILENAME **)malloc(sizeof(FILENAME *) * MAX_FILES+1);
	
	char prev_dir[1024];
	getcwd(prev_dir, sizeof(prev_dir));

	if(files == NULL){
		cout<<"Unable to allocate filename array, aborting..."<<endl;
		return 1;
	}
	cout<<"Listing for dir:"<<dir<<endl;
	if(strcmp(dir, " ") == 0){
		dir = ".";
	}
	if((dir_ptr = opendir(dir)) == NULL){
		printf("Cannot open directory %s\n", dir);
		return 1;
	}
	chdir(dir);
	while((cur_dir = readdir(dir_ptr)) != NULL){
		//printf("d_name:%s\n",cur_dir->d_name);
		if(cur_dir->d_name[0] == '.' && !show_all){
			continue;
		}
		if(lstat(cur_dir->d_name, &stat) < 0){
			perror("stat() failed:");
			exit(1);
		}
		if((temp_file = (FILENAME *)malloc(sizeof(FILENAME))) == NULL){
			printf("Cannot malloc filename %s\n", cur_dir->d_name);
			exit(1);
		}
		strcpy(temp_file->name, cur_dir->d_name);
		temp_file->fsize = stat.st_size;
		temp_file->mode = stat.st_mode;
		
		files[file_count++] = temp_file;
	}
	cout<<endl<<dir<<" contains:"<<endl;
	if(show_long){//print -l
	for(int i = 0; i < file_count; i++){
		sprintf(perms, "%c%c%c%c%c%c%c%c%c",
			(files[i]->mode & S_IRUSR) ? 'r' : '-',
			(files[i]->mode & S_IWUSR) ? 'w' : '-',
			(files[i]->mode & S_IXUSR) ? 'x' : '-',
			(files[i]->mode & S_IRGRP) ? 'r' : '-',
			(files[i]->mode & S_IWGRP) ? 'w' : '-',
			(files[i]->mode & S_IXGRP) ? 'x' : '-',
			(files[i]->mode & S_IROTH) ? 'r' : '-',
			(files[i]->mode & S_IWOTH) ? 'w' : '-',
			(files[i]->mode & S_IXOTH) ? 'x' : '-');
		printf("%s\t", perms);
		printf("%s\t%d\n", files[i]->name, files[i]->fsize);	
	}	
	}else{//std print
		for(int a = 0; a < file_count; a++){
			cout<<files[a]->name<<"\t";
		}
		cout<<endl;
	}
	
	chdir(prev_dir);
	closedir(dir_ptr);
}

void handle_myls(char *in, jobStruct * curJob){
	cout<<"Command:"<<in<<endl;

	bool show_long = false;
	bool show_all = false;
	
	int c;
	int index;
	int found_opts = 0;
	int this_args = curJob->argc;
	//cout<<"curJob argc:"<<curJob->argc<<endl;
	//cout<<"this_argc:"<<this_args<<endl;
	bool has_listed = false;
	while ((c = getopt(this_args, curJob->argv, "la")) != -1){
	switch (c){
           case 'a':
             show_all = true;
		found_opts++;
             break;
           case 'l':
             show_long = true;
		found_opts++;
             break;
           }
	}
        //cout<<"show_all:"<<show_all<<"  show_long:"<<show_long<<endl;
		for (index = optind; index < this_args; index++){
		//cout<<"curJob[iter]:"<<curJob->argv[index]<<endl;
			if(curJob->argv[index][0] != '-'){
				listDir(curJob->argv[index], show_long, show_all);
				has_listed = true;
			}
		}
		if(!has_listed){//list dot
			listDir(".", show_long, show_all);
		}
	show_long = false;
	show_all = false;
	optind = 0;
	
	//cout<<"handle_myls done..."<<endl;
}

int get_job_position(int job_n, vector<struct jobStore> &jobStore){
  int store_size = jobStore.size();
  for(int i = 0; i < store_size;i++){
    if(jobStore[i].job_n == job_n){
      return i;
    }
  }
  return -1;
}

void p_summary(){
	printf("\nJobs in total: %d\n",(cmd_n-1));
}

void p_jobs(){//TODO: rewrite
	change_status("Running");
	for(int n = 0; n < jobStore.size(); n++){//loop over jobs
		if(jobStore[n].is_bg && !jobStore[n].has_listed){//bg * shown		
			printf("Job[%d]*:\t%s ",jobStore[n].job_n,jobStore[n].cmd_name);
		//}else{
			//printf("Job[%d]:\t%s ",jobStore[n].job_n,jobStore[n].cmd_name);
		//}
		for(int c = 0; c<jobStore[n].realJob->argc; c++){
			printf("%s ", jobStore[n].realJob->argv[c]);
		}
		printf(" \t[%d] is currently: \t%s\n", jobStore[n].realJob->argc,jobStore[n].cmd_status);
		if(strcmp(jobStore[n].cmd_status, "Completed") == 0){
			jobStore[n].has_listed = true;//so we dont show again
		}
		}
	}	
	change_status("Completed");
}

void batch_m(int argc, char *fname){
	printf("batch mode.\n");	
	int arg_c = 0;
	char in[512];//will hold input cmd + args
	char *arg_v[10];//will hold parsed args
	char *cp; //temp to hold at parsing
	char *mj; //to hold multi job indication
	const char *delim = " \t\n"; //cmd delims
	int flag = 1;
	FILE *fin;
	fin = fopen(fname, "r");
	//loop
	do{
 		char buffer[512], *input;
		int cmdlen;
		
		printf("\n%s", PATH);//prompt
		flush_io();
		if(fgets(buffer, sizeof(buffer), fin)!=NULL){
			//printf("buf not null\n");
			//printf("buf is:%s\n",buffer);
			input = buffer;
		}else{
			printf("Batch complete...\n");
			input = "";
			break;
		}
		
		cmdlen = strlen(input);
		if(input[cmdlen-1] == '\n'){
			input[cmdlen-1] = '\0';
		}
		//printf("input is:%s",input);
		//printf("Job[%d]:%s", cmd_n, in);
		
		mj = strchr(input, ';');//check for multiple jobs on single line
		char *background_j = strchr(input, '&');//check for background jobs
		int bjn = 0;
		int mjn = 0;
		if(background_j != NULL){
			bjn = countchar(input, '&');
			printf("Detected %d jobs to run in background.\n", bjn);
			char *bjtok = strtok(input, "&");
			char *bjcmds[bjn];
			int bn = 0;
			while(bjtok != NULL){
				printf("[%s]\n", bjtok);
				bjcmds[bn] = bjtok;
				bjtok = strtok(NULL, "&");
				bn++;
			}
			int bjcount = 0;
			for(bjcount;bjcount<bjn;bjcount++){
				//printf("trying to handle: %s\n",bjcmds[bjcount]);
				flag = handle_job(bjcmds[bjcount], true);
			}
		}else if(mj != NULL){
			mjn = countchar(input, ';');
			printf("\nWe have detected %d jobs on the same input. \n",mjn+1);
			char *mjtok = strtok(input, ";");
			char *mjcmds[mjn];
			int cn = 0;
			while(mjtok != NULL){
				printf("[%s]\n", mjtok);
				mjcmds[cn] = mjtok;
				mjtok = strtok(NULL, ";");
				cn++;
			}
			int mjcount = 0;
			for(mjcount;mjcount<=mjn;mjcount++){
				//printf("trying to handle: %s\n",mjcmds[mjcount]);
				flag = handle_job(mjcmds[mjcount], false);
			}
		}else{
			flag = handle_job(input, false);
		}
	}while (flag == 1);
	fclose(fin);
	exit(0);
}

void std_m(){
	printf("standard mode.\n");	
	int arg_c = 0;
	char in[512];//will hold input cmd + args
	char *arg_v[10];//will hold parsed args
	char *cp; //temp to hold at parsing
	char *mj; //to hold multi job indication
	const char *delim = " \t\n"; //cmd delims
	int flag = 1;
	int valid_cmd = 0;
	//loop
	do{
 		char buffer[512], *input;
		int cmdlen;
		
		char directory[1024];
		printf("\n%s%s", getcwd(directory, sizeof(directory)),PATH);//prompt
		flush_io();
		if(fgets(buffer, sizeof(buffer), stdin)!=NULL){
			if(buffer[0] == '\n'){
				valid_cmd = 0;
			}else{
				input = buffer;
				valid_cmd = 1;
			}
		}else{
			printf("buf is null\n");
			input = "";
			break;
		}
		if(valid_cmd){
		cmdlen = strlen(input);
		if(input[cmdlen-1] == '\n'){
			input[cmdlen-1] = '\0';
		}
		//printf("input is:%s",input);
		//printf("Job[%d]:%s", cmd_n, in);
		
		mj = strchr(input, ';');//check for multiple jobs on single line
		char *background_j = strchr(input, '&');//check for background jobs
		int bjn = 0;
		int mjn = 0;
		if(background_j != NULL){
			bjn = countchar(input, '&');
			printf("Detected %d jobs to run in background.\n", bjn);
			char *bjtok = strtok(input, "&");
			char *bjcmds[bjn];
			int bn = 0;
			while(bjtok != NULL){
				printf("[%s]\n", bjtok);
				bjcmds[bn] = bjtok;
				bjtok = strtok(NULL, "&");
				bn++;
			}
			int bjcount = 0;
			for(bjcount;bjcount<bjn;bjcount++){
				//printf("trying to handle: %s\n",bjcmds[bjcount]);
				flag = handle_job(bjcmds[bjcount], true);
			}
		}else if(mj != NULL){
			mjn = countchar(input, ';');
			printf("\nWe have detected %d jobs on the same input. \n",mjn+1);
			char *mjtok = strtok(input, ";");
			char *mjcmds[mjn];
			int cn = 0;
			while(mjtok != NULL){
				printf("[%s]\n", mjtok);
				mjcmds[cn] = mjtok;
				mjtok = strtok(NULL, ";");
				cn++;
			}
			int mjcount = 0;
			for(mjcount;mjcount<=mjn;mjcount++){
				//printf("trying to handle: %s\n",mjcmds[mjcount]);
				flag = handle_job(mjcmds[mjcount], false);
			}
		}else{
			flag = handle_job(input, false);
		}
		}
	}while (flag == 1);
	exit(0);
}
int handle_job(char *input, bool bg){
	int shell_terminal = STDIN_FILENO;
	pid_t shell_pgid = getpid();
	int flag = 1;
	pid_t child;
	int childpid, status;
	
	int x = 1;
	char *cmd;
	
	char *command;
	int cmdlen;
			
	jobStruct *job = (jobStruct*)malloc(sizeof(jobStruct));
	job->argc = 1;
	
	//cout<<"cmd full is:"<<input<<endl;
	job->cmd_full = input;
	char in_saved[1024];
	strcpy(in_saved, input);
	cmd = strtok(input, " ");
	command = cmd;
	if(strcmp(command, "exit") == 0){
		flag = 0;
		p_summary();
		int i = 0;
		for(i; i < cmd_n; i++){
			exit(1);
		}
		return -1;//why isn't this exiting...
	}
	if(strcmp(command, "jobs") == 0){
		p_jobs();
		return flag;
	}
	job->cmd_path = command;
	job->argv[0] = job->cmd_path;
	while(1){
		cmd = strtok(NULL, " ");
		//printf("parsed: %s ", cmd);
		if(cmd == NULL){
			break;
		}
		job->argv[job->argc] = cmd;
		job->argc = job->argc+1;
	}
	struct jobStore tempStore;
	tempStore.realJob = job;
	tempStore.job_n = cmd_n;
	tempStore.cmd_status = "Scheduled";
	tempStore.cmd_name = command;
	tempSTore.has_listed = false;
	if(bg){
		tempStore.is_bg = true;
	}else{
		tempStore.is_bg = false;
	}
	jobStore.push_back(tempStore);
	
	int loc = get_job_position(cmd_n, jobStore);
	if(strcmp(command, "cd") == 0){
		handle_cd(in_saved,job);
		change_status("Completed");
		cmd_n++;
		return flag;
	}
	if(strcmp(command, "myls") == 0){
		handle_myls(in_saved, job);
		change_status("Completed");
		cmd_n++;
		return flag;
	}
	//printf("realJob->cmd_path: %s\n",jobStore[loc].realJob->cmd_path);
	
	childpid = fork();
	int get = tcgetpgrp(shell_terminal);
	
	if(childpid == 0){//done by child
		runJob(job, childpid);
	}else{//run by parent
		change_status("Waiting");
		if(!bg){
				childpid = waitpid(childpid, &status, 0); 
				if (childpid > 0){
                                        printf("waitpid got child pid %d\n", childpid);
				}
				change_status("Completed");
		}else{
			change_status("BG:Unknown");
		}
		jobStore[loc].realJob->cmd_path= cmd;
	}
	cmd_n++;
	return flag;
}

void change_status(char *status){
	int loc = get_job_position(cmd_n, jobStore);
	jobStore[loc].cmd_status = status;
}

void child(int argc, char *argv[10]) {
	printf("in child method\n");
  execvp(argv[0], argv);
}

int countchar(const char list[], char what ){
	int i=0;
	int count = 0;
	for( ; list[i] != 0 ; ++i ){
        if( list[i] == what ) ++count ;
	}
  return count;
}
void handle_signal(int signo){
	char directory[1024];
	printf("\n%s%s", getcwd(directory, sizeof(directory)),PATH);//prompt
	fflush(stdout);
}

int main(int argc, char *argv[]){
	system("clear");//clear anything currently oustanding
	flush_io();
	
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
	
	if(argc>1){
		char *fname = argv[1];
		batch_m(argc, fname);
	}else{
		std_m();
	}
	return 0;
}


