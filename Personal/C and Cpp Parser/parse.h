/*
** Indiana University | parse.h
** (c) Jonathan M. Stout
*/

/*
** Struct for each Flag to be stored
*/
typedef struct {
  char* name;
  bool value;
} Flag;

/*
** Struct for each option to be stored
*/
typedef struct {
  char* name;
  char* value;
} Option;

int _argc; // Number of arguments
char** _argv; // Arguments

struct Option all_options[MAX_OPTIONS]; // All stored Options
struct Flag all_flags[MAX_FLAGS];       // All stored Flags

/*
** Parses command arguments:
** Get commands passed in on
** launch.
*/
void parseCommandArgs(int argc, char** argv);

/*
** Parses command arguments:
** Get commands being passed in while
** program is running from stdin.
*/
void parseCurrentArgs();

/*
** Add argument:
** e.g. addOption("size") | e.g. addOption('s')
*/
void addOption(char* l_Opt);
void addOption(char s_Opt);

/*
** Add argument:
** e.g. addFlag("help") | e.g. addFlag('h')
*/
void addFlag(char* l_Flag);
void addFlag(char s_Flag);

/*
** Get option:
** getOption("size") | getOption('s)
*/
char* getOption(char* option);
char* getOption(char option);

/*
** Get flag:
** getFlag("help") | getFlag('h')
*/
bool getFlag(char* flag);
bool getFlag(char flag);
