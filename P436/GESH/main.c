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

#define MAX_LEN 512;
#define ARGS 10;

int cmd_n = 1;//keep track of # cmds
struct job jbs[512];


void p_summary(){
	printf("\nJobs in total: %d\n",(cmd_n-1));
}

void p_jobs(){
int n = 1;//start at index = 1
	for(n;n<cmd_n;n++){
		int jn = jbs[n].job_n;
		printf("Job[%d]:",jn);
		char *args[10];
		char *command = jbs[n].cmd_s;
		printf("%s",command);
		int an = 0;
		for(an; an < 10; an++){
			args[an] = jbs[an].cmd_a[an];
			printf(" %s",args[an]);
		}			
	}
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
		jbs[cmd_n].cmd_s = in;//add cmd to struct member
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
	//loop
	while(1){
		//show prompt
		printf("GE_sh@:");
		//read input
		if(fgets(in, sizeof(in), stdin) == NULL){//read from stdin and place into 'in'
			p_summary();//print summary
			break;
		}
		printf("Job[%d]:%s", cmd_n, in);
		mj = strchr(in, ';');//check for multiple jobs on single line
		if(mj != NULL){
			printf("\nWe have detected multiple jobs on the same input. TODO.\n");
		}
		//TODO: parse prompt
		cp = in;
		jbs[cmd_n].job_n = cmd_n;//add jobn to struct member
		jbs[cmd_n].cmd_s = in;//add cmd to struct member
		//if(in == "" || in == " " || in == '\n'){ break; }
		for (arg_c; arg_c < 10; arg_c++){//use const here
			if((arg_v[arg_c] = strtok(cp, delim)) == NULL){
				break;
			}
			jbs[cmd_n].cmd_a[arg_c] = arg_v[arg_c];//add arg to struct member
			cp = NULL;
		}
		//echo cmd + args (loop here) in formatted syntax
		if(strcmp(arg_v[0], "exit") == 0){//check for exit cmd
			p_summary();//print summary
			exit(0);
		}
		if(strcmp(arg_v[0], "jobs") == 0){//check for exit cmd
			p_jobs();//print jobs
		}
		cmd_n++;//increment job count
	}
}


int main(int argc, char *argv[]){
	if(argc>1){
		char *fname = argv[1];
		batch_m(argc, fname);
	}else{
		std_m();
	}
	return;
}
