#include <string>//to support strings
#include <iostream>
using namespace std;

class EndHosts{
protected://vars
	int eh_id;
	string ext_ip;
	string vir_ip;
public://methods
	EndHosts();
	EndHosts(int eh_id, string ext_ip, string vir_ip);
	
	//destructor
	//~EndHosts();
	
	int get_eh_id();
	string get_ext_ip();
	string get_vir_ip();
};

EndHosts::EndHosts(){
	this->eh_id = 0;
	this->ext_ip = "";
	this->vir_ip = "";
}
EndHosts::EndHosts(int ehid, string extip, string virip){
	this->eh_id = ehid;
	this->ext_ip = extip;
	this->vir_ip = virip;
}
int EndHosts::get_eh_id(){
	return this->eh_id;
}
string EndHosts::get_ext_ip(){
	return this->ext_ip;
}
string EndHosts::get_vir_ip(){
	return this->vir_ip;
}
