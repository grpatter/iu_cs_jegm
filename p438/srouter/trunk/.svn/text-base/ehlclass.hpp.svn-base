#include <string>//to support strings
using namespace std;
class Ehlinks{
public://vars
  int r_id;
  int delay;
  string vir_id;
  int eh_id;
public://methods
  Ehlinks();
  Ehlinks(int r_id, int delay, string vir_id, int eh_id);
  
  //destructor
  //~Links();
  
  int get_r_id();
  int get_delay();
  string get_vir_id();
  int get_eh_id();
  int get_eh_r_delay();
};

Ehlinks::Ehlinks(){
  this->r_id = -1;
  this->delay = -1;
  this->vir_id = "";
  this->eh_id = -1;
}
Ehlinks::Ehlinks(int r_id, int delay, string vir_id, int eh_id){
  this->r_id = r_id;
  this->delay = delay;
  this->vir_id = vir_id;
  this->eh_id = eh_id;
}

int Ehlinks::get_r_id(){
  return this->r_id;
}

int Ehlinks::get_delay(){
  return this->delay;
}

string Ehlinks::get_vir_id(){
  return this->vir_id;
}

int Ehlinks::get_eh_id(){
  return this->eh_id;
}
