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
	bool is_bg;
	struct jobStruct *realJob;
}; 

typedef struct jobStruct{
	char *cmd_path;
	int argc;
	char *argv[ARGS];
	char *cmd_full;
}jobStruct; 

struct filename{
	char name[512];
	off_t fsize;
	int links;
	uid_t uid;
	gid_t gid;
	mode_t mode;
	time_t mod_time;
	long file_blocks;
	dev_t dev_att;
};

