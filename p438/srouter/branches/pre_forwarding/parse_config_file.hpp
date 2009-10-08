#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>//to support strings
#include <vector>//to support vecs
#include <sstream>
#include <cctype>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/param.h>

#include "gconfig.hpp"
#include "hclass.hpp"
#include "rclass.hpp"
#include "lclass.hpp"
#include "ehlclass.hpp"
#include "p438_server.hpp"
#include "virhead.hpp"

using namespace std;

void token_str(vector<string>& tokens, string str);//prototypes
string get_time();
unsigned long this_ip();
void parse_globals(vector<string> line_vals);
void parse_routers(vector<string> line_vals);
void parse_hosts(vector<string> line_vals);
void parse_rrlinks(vector<string> line_vals);
void parse_rhlinks(vector<string> line_vals); 
void read_cfg();

Global_Config cfg;
vector<Routers> r_inst;
vector<EndHosts> h_inst;
vector<Links> rr_inst;
vector<Ehlinks> rh_inst;

unsigned long this_ip(){
  char hostname[MAXHOSTNAMELEN];
  gethostname(hostname, MAXHOSTNAMELEN);
  cout<<"host name: "<<hostname<<endl;
  hostent * record = gethostbyname(hostname);
  if (record == NULL){
    herror("gethostbyname failed.");
    exit(1);
  }
  in_addr * address = (in_addr *)record->h_addr;
  cout<<"Main Address:"<<inet_ntoa(* address)<<endl;
  return (unsigned long)inet_ntoa(* address);
}

struct vir_head read_send_file(){
  ifstream in_stream;
  string cur_line;
  vector<string> header_values;
  int dest_port;
  char *data;
  int length;
  struct udp_struct tmp_udp_head;
  struct vir_head tmp_vir_head;

  //search for send.txt file
  in_stream.open("send.txt");
  if(in_stream.fail()) {
    cout<<"Send file error."<<endl;//nothing to send
    exit(1);
  }

  getline(in_stream, cur_line);//first line has header values

  //parse through the first line of send file
  token_str(header_values, cur_line);
  tmp_vir_head.ip4_str = header_values[0];
  tmp_udp_head.src_port = atoi(header_values[1].c_str());
  tmp_udp_head.dest_port = atoi(header_values[2].c_str());
  tmp_vir_head.udphd = tmp_udp_head;
  
  //get length of reamining data and read in from file store in data
  in_stream.seekg(0, ios::end);
  length = in_stream.tellg();
  in_stream.seekg(0, ios::beg);
  data = new char [length];
  in_stream.read(data, length);

  tmp_vir_head.data = data;
  in_stream.close();

  return tmp_vir_head;
}
void read_cfg(){
//int main(int argc, char* argv[]){
  //ofstream out_stream;
  ifstream in_stream;
  string cur_line;
  string in_file;
  static int rnum = 0;
  static int hnum = 0;
  static int rrnum = 0;
  static int rhnum = 0;
  vector<string> line_vals;
  
  cout<<"Please enter the name of the config file(WITHOUT extension): ";
  cin>>in_file;//get name
  in_file += ".txt";
  in_stream.open(in_file.c_str());
  if(in_stream.fail()){
    cout<<"Config file opening failed. Exiting."<<endl;
    exit(1);
  }else{  
	cout<<"Now reading configuration data."<<endl;
  }
  //while(!in_stream.eof()){
  while(getline(in_stream,cur_line)){
    //getline(in_stream, cur_line);//get entire line
    token_str(line_vals, cur_line);//tokenize the string
    int type = atoi(line_vals[0].c_str());
    switch(type){
    case 0:
      parse_globals(line_vals);
      cout<<"globals created."<<endl;
      break;
    case 1:
      parse_routers(line_vals);
      cout<<"routers updated"<<endl;
      break;
    case 2:
      parse_hosts(line_vals);
      cout<<"endhosts updated"<<endl;
      break;
    case 3:
      parse_rrlinks(line_vals);
      cout<<"rrlinks updated"<<endl;
      break;
    case 4:
      parse_rhlinks(line_vals);
      //we have a problem here, we are getting a seg fault error
      cout<<"case 4"<<endl;
      cout<<"rhlinks updated"<<endl;
      break;
    default:
      cout<<"Invalid type given. Reading next line."<<endl;
      break;
    }
    line_vals.clear();
    cout << "error3" << endl;
  }
  
  cout<<"***"<<endl;
  cout<<"cfg qlen: "<<cfg.get_queue_len()<<endl;
  cout<<"cfg ttlval: "<<cfg.get_ttl_val()<<endl;
  cout<<"***"<<endl;
  for(int ri = 0; ri < r_inst.size(); ri++){
    cout<<"routers[ri] rid: "<<r_inst[ri].get_r_id()<<" extip: "<<r_inst[ri].get_ext_ip()<<endl;
  }
  cout<<"***"<<endl;
  for(int hi = 0; hi < h_inst.size(); hi++){
    cout<<"hosts[hi] ehid: "<<h_inst[hi].get_eh_id()<<" ex/vir tip: "<<h_inst[hi].get_ext_ip()<<"/"<<h_inst[hi].get_vir_ip()<<endl;
  }
  cout<<"***"<<endl;
  for(int li = 0; li < rr_inst.size(); li++){
    cout<<"router links[li] r1id: "<<rr_inst[li].get_r1_id()<<" r1 delay: "<<rr_inst[li].get_r1_delay()
	<<"\tr2id: "<<rr_inst[li].get_r2_id()<<" r2 delay: "<< rr_inst[li].get_r2_delay() << endl;
  }
  cout<<"***"<<endl;
  for(int eli = 0; eli < rh_inst.size(); eli++){
    cout<<"router/endhost links[eli] rid: "<<rh_inst[eli].get_r_id()<<" r delay: "<<rh_inst[eli].get_r_eh_delay()
	<<" virtual id: "<< rh_inst[eli].get_vir_id()<<"\tehid: "<<rh_inst[eli].get_eh_id()<<" eh delay: "<< rh_inst[eli].get_eh_r_delay() << endl;
  }
  cout<<"***"<<endl;
	this_ip();
	cout<<"***"<<endl;
  //out_stream.close();
  in_stream.close();
  //return 0;
}

void token_str(vector<string>& tokens, string str){
  string buf; //prep a buffer
  stringstream ss(str); //insert string into stream
  while(ss >> buf){
    tokens.push_back(buf);//add to vector
  }
}

void parse_globals(vector<string> line_vals){
  int qlen = atoi(line_vals[1].c_str());
  int ttlval = atoi(line_vals[2].c_str());	
  Global_Config cfg_r(qlen,ttlval);
  cfg = cfg_r;
}

void parse_routers(vector<string> line_vals){
  int r_id = atoi(line_vals[1].c_str());
  string ext_ip = line_vals[2];	
  Routers tmp_r(r_id,ext_ip);
  r_inst.push_back(tmp_r);
}

void parse_hosts(vector<string> line_vals){
  int eh_id = atoi(line_vals[1].c_str());
  string ext_ip = line_vals[2];	
  string vir_ip = line_vals[3];	
  EndHosts tmp_h(eh_id,ext_ip, vir_ip);
  h_inst.push_back(tmp_h);
}

void parse_rrlinks(vector<string> line_vals){
  int r1_id = atoi(line_vals[1].c_str());
  int r1_delay = atoi(line_vals[2].c_str());	
  int r2_id = atoi(line_vals[3].c_str());	
  int r2_delay = atoi(line_vals[4].c_str());	
  Links tmp_l(r1_id, r1_delay, r2_id, r2_delay);
  rr_inst.push_back(tmp_l);
}

void parse_rhlinks(vector<string> line_vals){
  int r_id = atoi(line_vals[1].c_str());
  int r_eh_delay = atoi(line_vals[2].c_str());	
  string vir_id = line_vals[3];
  int eh_id = atoi(line_vals[4].c_str());	
  int eh_r_delay = atoi(line_vals[5].c_str());	
  Ehlinks tmp_eh_l(r_id, r_eh_delay, vir_id, eh_id, eh_r_delay);
  rh_inst.push_back(tmp_eh_l);
}
