
#define ARGS 10

struct jobStore{
	int job_n;
	char *cmd_a[ARGS];
	char *cmd_s;
	char *cmd_status;
	struct jobStruct *jbs;
}; 

typedef struct jobStruct{
	char *cmd_path;
	int argc;
	char *argv[ARGS];
	char cmd_suffix;
}jobStruct; 
