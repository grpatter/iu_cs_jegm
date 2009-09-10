#include <string>//to support strings
using namespace std;
class Links{
protected://vars
  int r1_id;
  int r1_delay;
  int r2_id;
  int r2_delay;
public://methods
  Links();
  Links(int r1_id, int r1_delay, int r2_id, int r2_delay);
  
  //destructor
  //~Links();
  
  int get_r1_id();
  int get_r2_id();
  int get_r1_delay();
  int get_r2_delay();
};

Links::Links(){
  this->r1_id = -1;
  this->r1_delay = -1;
  this->r2_id = -1;
  this->r2_delay = -1;
}
Links::Links(int r1_id, int r1_delay, int r2_id, int r2_delay) {
  this->r1_id = r1_id;
  this->r1_delay = r1_delay;
  this->r2_id = r2_id;
  this->r2_delay = r2_delay;
}

int Links::get_r1_id(){
  return this->r1_id;
}
int Links::get_r1_delay(){
  return this->r1_delay;
}

int Links::get_r2_id(){
  return this->r2_id;
}
int Links::get_r2_delay(){
  return this->r2_delay;
}
