#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Set the following port to a unique number:
#define MYPORT 5000

void error(char *s) {
  perror(s);
  exit(0);
}

int create_p438_socket() {
  int sock;
  struct sockaddr_in server;
  
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  
    if (sock < 0) error("Error creating P438 socket");
    
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(MYPORT);
    if (bind(sock, (struct sockaddr *) &server, sizeof(server) ) < 0) 
      error("Unable to bind P438 socket");
    
    // Socket is now bound:
    return sock;
}

int p438_recv(int sock, char *buffer, int buff_size) {
  struct sockaddr_in from;
  int fromlen, n;
  fromlen = sizeof(struct sockaddr_in);
  n = recvfrom(sock, buffer, buff_size, 0,(struct sockaddr *)&from, (socklen_t*)&fromlen);
  return n;
}

int p438_send(int sock, char *buffer, int buff_size, unsigned long nextIP) {
  struct sockaddr_in to;
  int tolen, n;
  
  tolen = sizeof(struct sockaddr_in);
  
  // Okay, we must populate the to structure. 
  bzero(&to, sizeof(to));
  to.sin_family = AF_INET;
  to.sin_port = htons(MYPORT);
  to.sin_addr.s_addr = nextIP;
  
  // We can now send to this destination:
  n = sendto(sock, buffer, buff_size, 0,
	     (struct sockaddr *)&to, tolen);
  
  return n;
}

