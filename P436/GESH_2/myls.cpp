#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <termios.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include "jobs.h"
#include <sys/stat.h>
#include <errno.h>

using namespace std;
typedef struct filename FILENAME;

#define MAX_LEN 512
#define MAX_FILES 16324
#define SEC_PER_YEAR	60 * 60 * 24 * 365

int print_long(FILENAME *f){
	char		ftype;		/*  File type -, d, p, c, b, l */
	char		perm[16];	/*  Permission string (-rwxrwxrwx) */

	char		link[256];	/*  Target of symbolic link */

	char		owner[32];	/*  Name of file's owner */
	char		group[32];	/*  Name of file's group owner */
	struct passwd 	*pw;		/*  Temporary passwd structure */
	struct group	*gr;		/*  Temporary group structure */

	char		mtime[32];	/*  Formatted modification time */
	time_t		now;		/*  Current timestamp */
	struct tm	*tm, *tnow;	/*  Temporary time structures */

	/*  Determine the file's type */	
	if (S_ISREG(f->mode))
		ftype = '-';
	else if (S_ISDIR(f->mode))
		ftype = 'd';
	else if (S_ISLNK(f->mode))
		ftype = 'l';
	else if (S_ISFIFO(f->mode))
		ftype = 'p';
	else if (S_ISBLK(f->mode))
		ftype = 'b';
	else if (S_ISCHR(f->mode))
		ftype = 'c';
	else if (S_ISSOCK(f->mode))
		ftype = 's';

	/*  If we have a symlink, then lookup the target filename */
	if (S_ISLNK(f->mode)){
		if (readlink(f->name, link, 256) >= 0){
			sprintf(f->name, "%s -> %s", f->name, link);
		}	
	}
	
	/*  Decode the permission string */
	sprintf(perm, "%c%c%c%c%c%c%c%c%c%c",ftype,
		(f->mode & S_IRUSR) ? 'r' : '-',
		(f->mode & S_IWUSR) ? 'w' : '-',
		(f->mode & S_IXUSR) ? 'x' : '-',
		(f->mode & S_IRGRP) ? 'r' : '-',
		(f->mode & S_IWGRP) ? 'w' : '-',
		(f->mode & S_IXGRP) ? 'x' : '-',
		(f->mode & S_IROTH) ? 'r' : '-',
		(f->mode & S_IWOTH) ? 'w' : '-',
		(f->mode & S_IXOTH) ? 'x' : '-');

	/*  Make corrections for SUID, SGID, and sticky bit */
	if (f->mode & S_ISUID)
		perm[3] = (f->mode & S_IXUSR) ? 's' : 'S';
	if (f->mode & S_ISGID)
		perm[6] = (f->mode & S_IXGRP) ? 's' : 'S';
	if (f->mode & S_ISVTX)
		perm[9] = (f->mode & S_IXOTH) ? 't' : 'T';

	/*  Use the uid to get the owner's name */
	if ((pw = getpwuid(f->uid)) == NULL)
		sprintf(owner, "%d", f->uid);
	else
		strcpy(owner, pw->pw_name);	

	/*  Use the gid to get the group owner's name */
	if ((gr = getgrgid(f->gid)) == NULL)
		sprintf(group, "%d", f->gid);
	else
		strcpy(group, gr->gr_name);	

	/*  Format the modification time */
	now = time(0);
	tnow = localtime(&now);
	tm = localtime(&(f->mod_time));
	if (f->mod_time < now - SEC_PER_YEAR)
		strftime(mtime, 32, "%b %d  %Y", tm);
	else
		strftime(mtime, 32, "%b %d %H:%M", tm);
	
	/*  Print the output */
	if (S_ISCHR(f->mode) || S_ISBLK(f->mode)){
		printf("%s %4d %-8s %-9s %3d,%3d %s %s\n", 
			perm, f->links, owner, group, major(f->dev_att), minor(f->dev_att), mtime, f->name);
	}else{
		printf("%s %4d %-8s %-9s %7d %s %s\n", 
			perm, f->links, owner, group, f->fsize, mtime, f->name);	
	}
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
	//cout<<"Listing for dir:"<<dir<<endl;
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
		temp_file->mod_time = stat.st_mtime;
		temp_file->uid = stat.st_uid;
		temp_file->gid = stat.st_gid;
		temp_file->links = stat.st_nlink;
		temp_file->file_blocks = stat.st_blocks;
		temp_file->dev_att = stat.st_rdev;		
		files[file_count++] = temp_file;
	}
	cout<<endl<<dir<<" contains:"<< file_count <<" files..."<<endl;
	
	//qsort(files, file_count, sizeof(FILENAME *), cmp_name);
	if(show_long){//print -l
	for(int i = 0; i < file_count; i++){
		print_long(files[i]);
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

void handle_myls(int argc, char *argv[]){
	bool		showall = false;	/*  "-s" flag */
	bool		showlong = false;	/*  "-l" flag */
	char		c;			/*  Current option */
	char		dir[128];		/*  Target directory */

	/*  Process command line args */
	while ((c = getopt(argc, argv, "al")) != -1)
	{
		switch (c)
		{
			case 'a':		
				showall = true;
				break;
			case 'l':		
				showlong = true;
				break;
			case '?':	
				fprintf(stderr, "Invalid option -%c\n",
					optopt);
				exit(1);
				break;
		}
	}

	/*  After removing the options, we should only have dir left */
	if (argc - optind > 1)
	{
		fprintf(stderr, "usage: myls [-slt] [dir]\n");
		exit(1);
	}

	/*  If there is no dir, then use "." */
	if (argc == optind)
		strcpy(dir, ".");
	else
		strcpy(dir, argv[optind]);


	/*  Call the myls function to do the actual work */
	listDir(dir, showlong, showall);

	/*  Cleanup */
	exit(0);
}

int main(int argc, char *argv[]){
	system("clear");//clear anything currently oustanding
	
	handle_myls(argc, argv);
	return 0;
}