#include <string>//to support strings
using namespace std;
class Routers{
public://vars
  int r_id;
  string ext_ip;
public://methods
  Routers();
  Routers(int r_id, string ext_ip);
  
  //destructor
  //~Routers();
  
  int get_r_id();
  string get_ext_ip();
};

Routers::Routers(){
  this->r_id = 0;
  this->ext_ip = "";
}
Routers::Routers(int rid, string extip){
  this->r_id = rid;
  this->ext_ip = extip;
}
int Routers::get_r_id(){
  return this->r_id;
}
string Routers::get_ext_ip(){
  return this->ext_ip;
}
