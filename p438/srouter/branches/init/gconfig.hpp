#include <string>//to support strings
using namespace std;
class Global_Config{
protected://vars
	int queue_len;
	int ttl_val;
public://methods
	Global_Config();
	Global_Config(int queue_len, int ttl_val);
	
	//destructor
	//~Global_Config();
	
	int get_queue_len();
	int get_ttl_val();
};

Global_Config::Global_Config(){
	this->queue_len = 0;
	this->ttl_val = 0;
}

Global_Config::Global_Config(int qlen, int ttl){
	this->queue_len = qlen;
	this->ttl_val = ttl;
}

int Global_Config::get_queue_len(){
	return this->queue_len;
}

int Global_Config::get_ttl_val(){
	return this->ttl_val;
}
