//Greg Patterson
//grpatter at indiana.edu
//Eric Spencer
//erspence at indiana.edu
//main for custom shell impl
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jobs.h"

#define MAX_LEN 512
int countchar(const char[], char);

int cmd_n = 1;//keep track of # cmds
struct jobStore jbs[50];
char * PATH = "[GE_SH@]$ ";
char *path;

int runJob(jobStruct * job);
void flush_io();

void flush_io(){
	fflush(stdin);
	fflush(stdout);
}

int runJob(jobStruct * job){
	printf("Command: %s\n", job->cmd_path);
	change_status("Running");
	jbs[cmd_n].cmd_s = job->cmd_path;
	execvp(job->cmd_path, job->argv);
	change_status("Completed");
	return 1;
}

void p_summary(){
	printf("\nJobs in total: %d\n",(cmd_n-1));
}

void p_jobs(){
int n = 1;//start at index = 1
	change_status("Running");
	for(n; n < cmd_n; n++){//loop over jobs
		int arg_c = 0;
		printf("Job[%d]:%s [%d] is currently: %s\n",jbs[n].job_n,jbs[n].jbs->cmd_path,jbs[n].jbs->argc,jbs[n].cmd_status);
	}
	change_status("Completed");
}

void batch_m(int argc, char *fname){
	printf("batch.\n");
	int arg_c = 0;
	char in[512];//will hold input cmd + args
	char *arg_v[10];//will hold parsed args
	char *cp; //temp to hold at parsing
	char *mj; //to hold multi job indication
	const char *delim = " \t\n"; //cmd delims
	FILE *fin;
	fin = fopen(fname, "r");
	while(1){
		if(fgets(in, sizeof(in), fin) == NULL){
			p_summary();//print summary
			break;
		}
		printf("Job[%d]:%s", cmd_n, in);
		mj = strchr(in, ';');
		if(mj != NULL){
			printf("\nWe have detected multiple jobs on the same input (batch). TODO.\n");
		}
		//TODO: parse prompt
		cp = in;
		jbs[cmd_n].job_n = cmd_n;//add jobn to struct member
		jbs[cmd_n].cmd_s = "test";//add cmd to struct member
		//if(in == "" || in == " " || in == '\n'){ break; }
		for (arg_c; arg_c < 10; arg_c++){//use const here
			if((arg_v[arg_c] = strtok(cp, delim)) == NULL){
				break;
			}
			jbs[cmd_n].cmd_a[arg_c] = arg_v[arg_c];//add arg to struct member
			cp = NULL;
		}
		//echo cmd + args (loop here)
		if(strcmp(arg_v[0], "exit") == 0){//check for exit cmd
			p_summary();//print summary
			fclose(fin);
			exit(0);
		}
		cmd_n++;//increment job count
	}
	fclose(fin);
}

void std_m(){
	printf("standard.\n");	
	int arg_c = 0;
	char in[512];//will hold input cmd + args
	char *arg_v[10];//will hold parsed args
	char *cp; //temp to hold at parsing
	char *mj; //to hold multi job indication
	const char *delim = " \t\n"; //cmd delims
	int flag = 1;
	//loop
	do{
 		char buffer[512], *input;
		int cmdlen;
		
		printf("\n%s", PATH);//prompt
		flush_io();
		if(fgets(buffer, sizeof(buffer), stdin)!=NULL){
			printf("buf not null\n");
			printf("buf is:%s\n",buffer);
			input = buffer;
		}else{
			printf("buf is null\n");
			input = "";
			break;
		}
		
		cmdlen = strlen(input);
		if(input[cmdlen-1] == '\n'){
			input[cmdlen-1] = '\0';
		}
		printf("input is:%s",input);
		//printf("Job[%d]:%s", cmd_n, in);
		
		mj = strchr(input, ';');//check for multiple jobs on single line
		int mjn = 0;
		if(mj != NULL){
			mjn = countchar(input, ';');
			printf("\nWe have detected %d jobs on the same input. TODO.\n",mjn+1);
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
				printf("trying to handle: %s\n",mjcmds[mjcount]);
				handle_job(mjcmds[mjcount]);
			}
		}else{
			flag = handle_job(input);
		}
	}while (flag == 1);
	exit(0);
}
int handle_job(char *input){
	int flag = 1;
	pid_t child;
	int childstatus, status;
	
	int x = 1;
	char *cmd;
	
	char *command;
	int cmdlen;
			
	jobStruct *job = (jobStruct*)malloc(sizeof(jobStruct));
	job->argc = 1;
	
	cmd = strtok(input, " ");
	command = cmd;
	if(strcmp(command, "exit") == 0){
		flag = 0;
		p_summary();
		int i = 0;
		for(i; i < cmd_n; i++){
			exit(1);
		}
		return;//why isn't this exiting...
	}
	if(strcmp(command, "jobs") == 0){
		p_jobs();
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
	//printf("\nNum args: %d\n",job->argc);		
	jbs[cmd_n].jbs = job;
	jbs[cmd_n].job_n = cmd_n;
	jbs[cmd_n].cmd_s = input;
	printf("cmd_s: %s\n",jbs[cmd_n].cmd_s);
	jbs[cmd_n].cmd_status = "Scheduled";
	
	childstatus = fork();
	if(childstatus != 0){
		change_status("Waiting");
		wait(&status);
		jbs[cmd_n].cmd_s = cmd;
		change_status("Completed");
	}else{
		runJob(job);
	}
	cmd_n++;
	return flag;
}

void change_status(char *status){
	jbs[cmd_n].cmd_status = status;
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

int main(int argc, char *argv[]){
	system("clear");//clear anything currently oustanding
	flush_io();
	if(argc>1){
		char *fname = argv[1];
		batch_m(argc, fname);
	}else{
		std_m();
	}
	return;
}

