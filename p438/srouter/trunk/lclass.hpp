#include <string>//to support strings
using namespace std;
class Links{
public://vars
  int r1_id;
  int delay;
  int r2_id;
public://methods
  Links();
  Links(int r1_id, int r1_delay, int r2_id);
  
  //destructor
  //~Links();
  
  int get_r1_id();
  int get_r2_id();
  int get_delay();
};

Links::Links(){
  this->r1_id = -1;
  this->delay = -1;
  this->r2_id = -1;
}
Links::Links(int r1_id, int delay, int r2_id) {
  this->r1_id = r1_id;
  this->delay = delay;
  this->r2_id = r2_id;
}

int Links::get_r1_id(){
  return this->r1_id;
}
int Links::get_delay(){
  return this->delay;
}

int Links::get_r2_id(){
  return this->r2_id;
}
