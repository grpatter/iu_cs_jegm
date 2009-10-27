#include <string>//to support strings
using namespace std;
class Ehlinks{
protected://vars
  int r_id;
  int r_eh_delay;
  string vir_id;
  int eh_id;
  int eh_r_delay;
public://methods
  Ehlinks();
  Ehlinks(int r_id, int r_eh_delay, string vir_id, int eh_id, int eh_r_delay);
  
  //destructor
  //~Links();
  
  int get_r_id();
  int get_r_eh_delay();
  string get_vir_id();
  int get_eh_id();
  int get_eh_r_delay();
};

Ehlinks::Ehlinks(){
  this->r_id = -1;
  this->r_eh_delay = -1;
  this->vir_id = "";
  this->eh_id = -1;
  this->eh_r_delay = -1;
}
Ehlinks::Ehlinks(int r_id, int r_eh_delay, string vir_id, int eh_id, int eh_r_delay){
  this->r_id = r_id;
  this->r_eh_delay = r_eh_delay;
  this->vir_id = vir_id;
  this->eh_id = eh_id;
  this->eh_r_delay = eh_r_delay;
}

int Ehlinks::get_r_id(){
  return this->r_id;
}

int Ehlinks::get_r_eh_delay(){
  return this->r_eh_delay;
}

string Ehlinks::get_vir_id(){
  return this->vir_id;
}

int Ehlinks::get_eh_id(){
  return this->eh_id;
}
int Ehlinks::get_eh_r_delay(){
  return this->eh_r_delay;
}
