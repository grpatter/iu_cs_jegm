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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "parse_config_file.hpp"
#include "time_func.hpp"

#define BUFLEN 512
#define NPACK 10
#define PORT 8558
#define SRV_IP "129.79.247.5"
static ofstream out_stream;

void rmode();
void hmode();
void diep(char *s);
void msleep(int s);

using namespace std;

void msleep(int s){
  usleep(s * 1000);
}

int main(int argc, char* argv[]){
  //admin log stuff
  string log_name = "AdminLog";
  log_name += file_name_time();
  out_stream.open(log_name.c_str());
  if(out_stream.fail()){
    cout<<"Output file opening failed.\n";
  }
  out_stream<<"BEGIN Administrative log for Virtual Router. (C) Greg Patterson and Eric Spencer 2009 \n"<<endl;
  cout<<"Starting up SoftRouter Network..."<<endl;
  read_cfg();
  cout<<"Configuration processing complete. Now determining machine IP..."<<endl;
  this_ip();
  cout<<"Now determining host or router mode..."<<endl;
  //this section is just for testing
  //we will actually do a lookup here to determine status automagically.
  int mode = 0;
  cout<<"Should this be a Router (0) or an EndHost (1)?: ";
  cin>>mode;
  if(mode == 0){
    cout<<"Now starting Router mode..."<<endl;
    rmode();
  }else{
    cout<<"Now starting EndHost mode..."<<endl;
    hmode();
  }	
  //end testing section
  out_stream.close();
  return 0;
}

void diep(char *s){
  perror(s);
  exit(1);
}

void rmode(){
  out_stream<<"Router mode initiated. Attempting a socket bind request..."<<endl;
  char buf[BUFLEN];
  struct sockaddr_in si_me, si_other;
  int s, i, slen=sizeof(si_other);
  s = create_p438_socket();
  while(1){
    int n = p438_recv(s, buf, BUFLEN);
    cout << "size received: " << n << endl;
    if (n==-1) diep("recvfrom()");
    cout <<"Received packet at: "<< get_time() <<endl; 
    printf("Received packet from %s:%d\nData: %s\n\n", 
	   inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
    if(n > 0) {
		cout<<"Attempting to reply."<<endl;
		  si_other.sin_family = AF_INET;
		  //si_other.sin_port = htons(PORT);
		char *target = "212.85.30.0";
		int size = p438_send(s, buf, BUFLEN, inet_aton(target, &si_other.sin_addr));
		cout<<"Send attempt finished. size =: "<< size <<endl;
    }
  }
  close(s);
}
				    
void hmode(){
  out_stream<<"EndHost mode initiated. Attempting a socket bind request..."<<endl;
  struct sockaddr_in si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];
  struct vir_head tmp_head;
  tmp_head = read_send_file();
  string data = tmp_head.data;
  string ip = tmp_head.ip4_str;
  int src_port = tmp_head.udphd.src_port;
  int dest_port = tmp_head.udphd.dest_port;

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) diep("socket");
  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }
   
  msleep(1000);//sleep 1 second
  //send stuff
  cout << "Sending data: \n" << data << endl;
  sprintf(buf, data.c_str(), 1);
  int n = p438_send(s, buf, BUFLEN, si_other.sin_addr.s_addr);
  cout << "size sent: " << n << endl;
  if(n==-1) diep("sendto()");
	
	close(s);
	s = create_p438_socket();
  //receive stuff
	char buf_r[BUFLEN];
    while(1){	
    int n = p438_recv(s, buf_r, BUFLEN);
    cout << "size received: " << n << endl;
    if (n==-1) diep("recvfrom()");
    cout <<"Received packet at: "<< get_time() <<endl; 
    printf("Received packet from %s:%d\nData: %s\n\n", 
	   inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf_r);
	}
  close(s);
}
