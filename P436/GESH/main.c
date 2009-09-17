//Greg Patterson
//grpatter at indiana.edu
//Eric Spencer
//erspence at indiana.edu
//main for custom shell impl
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 512;
#define ARGS 10;
int main(){
	int cmd_n = 1;//keep track of # cmds
	int arg_c = 0;
	char in[512];//will hold input cmd + args
	char *arg_v[10];//will hold parsed args
	char *cp; //temp to hold at parsing
	const char *delim = " \t\n"; //cmd delims
	//loop
	while(1){
		//show prompt
		printf("GE_sh@:");
		//read input
		if(fgets(in, sizeof(in), stdin) == NULL){//read from stdin and place into 'in'
			break;
		}
		printf("%s",in);
		//TODO: parse prompt
		cp = in;
		if(in == "" || in == " " || in == '\n'){ break; }
		for (arg_c; arg_c < 10; arg_c++){//use const here
			if((arg_v[arg_c] = strtok(cp, delim)) == NULL){
				break;
			}
			cp = NULL;
		}
		//TODO: check for exit
		if(strcmp(arg_v[0], "exit") == 0){//check for exit cmd
			//TODO: print summary
			printf("Job in total: %d",cmd_n-1);
			exit(0);
		}
		cmd_n++;//increment job count
	}
}
