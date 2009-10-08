
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
#include "gconfig.hpp"
#include "hclass.hpp"
#include "rclass.hpp"

void token_str(vector<string>& tokens, string str);//prototypes
string get_time();
void parse_globals(vector<string> line_vals);
void parse_routers(vector<string> line_vals);
void parse_hosts(vector<string> line_vals);
void parse_rrlinks(vector<string> line_vals);
void parse_rhlinks(vector<string> line_vals); 

Global_Config cfg;
vector<Routers> r_inst;
vector<EndHosts> h_inst;
//vector<> rr_inst;
//vector<> rh_inst;
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

int main(int argc, char* argv[]){
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
	}
	
	while(!in_stream.eof()){		
		getline(in_stream, cur_line);//get entire line
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
				//parse_rrlinks(line_vals);
				cout<<"rrlinks updated"<<endl;
				break;
			case 4:
				//parse_rhlinks(line_vals);
				cout<<"rhlinks updated"<<endl;
				break;
			default:
				cout<<"Invalid type given. Reading next line."<<endl;
				break;
		}
		line_vals.clear();
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
    //out_stream.close();
    in_stream.close();
	return 0;
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

