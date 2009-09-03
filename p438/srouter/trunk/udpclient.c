#define SRV_IP "129.79.247.5"
/* diep(), #includes and #defines like in the server */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <string>
#include "srouterStart.cpp"
#define BUFLEN 512
#define NPACK 10
#define PORT 9930

void diep(char *s){
	perror(s);
	exit(1);
}
int main(void){

	ifstream in_stream;
	string in_file;
	in_stream.open("send.txt");
	if(in_stream.fail()){
	cout<<"Send file not found. No message transmitted."<<endl;
	exit(1);
	}
	string cur_line;
	vector<string> send_args;
	getline(in_stream, cur_line);//get entire line
	token_str(send_args, cur_line);//tokenize the string
	//PORT = atoi(send_args[1].c_str());
	
	struct sockaddr_in si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];
	char buf2[BUFLEN];

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	  diep("socket");

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
	  fprintf(stderr, "inet_aton() failed\n");
	  exit(1);
	}

	for (i=0; i<NPACK; i++) {
	  printf("Sending packet %d\n", i);
	  sprintf(buf, "CL: This is packet %d\n", i);
	  if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
		diep("sendto()");
		
	
	  if (recvfrom(s, buf2, BUFLEN, 0, &si_other, &slen)==-1)
		diep("recvfrom()");
	  printf("Received packet from %s:%d\nData: %s\n\n", 
			inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
	}
	close(s);
	return 0;
}
