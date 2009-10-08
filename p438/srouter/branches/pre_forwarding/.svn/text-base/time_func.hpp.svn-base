#include <stdlib.h>
#include <string>
#include <time.h>
#include <iostream>

using namespace std;

string get_time() {
  //MM/DD/YY HH:MM:SS
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];  
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (buffer, 80, "%m/%d/%y %H:%M:%S", timeinfo);
  return buffer;
}

string file_name_time() {
  //DD MM
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];
  
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (buffer, 80, "%d%M.txt", timeinfo);
  return buffer;
}
