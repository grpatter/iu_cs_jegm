#include <iostream>
#include <string>

#define ARGS 10

struct jobStore{
	int job_n;
	//char *cmd_a[ARGS];
	//char *cmd_s;
	char *cmd_status;
	char *cmd_name;
	//string cmd_location;
	struct jobStruct *realJob;
}; 

typedef struct jobStruct{
	char *cmd_path;
	int argc;
	char *argv[ARGS];
}jobStruct; 

