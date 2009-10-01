/**********************************************************************
 * server.c --- Demonstrate a simple iterative server.
 * Tom Kelliher
 *
 * This program demonstrates a simple iterative server.  The server
 * opens a TCP connection on port SERVER_PORT and begins accepting
 * connections from anywhere.  It sits in an endless loop, so one must
 * send an INTR to terminate it.
 *
 * The server reads a message from the client, printing it to stdout.
 * Then, the server sends a simple message back to the client.
 **********************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA ":"
#define TRUE 1
#define SERVER_PORT 5016
#define BUFFER_SIZE 1024

/* prototypes */
void die(const char *);
void pdie(const char *);
void cConnect(char *buf, char *address);
void cMessage(char *buf);
void cWho(char *buf);
void cJoin(char *buf);
void cPart(char *buf);
void cQuit(char *buf, char *address, int *msgsock);

/**********************************************************************
 * main
 **********************************************************************/
int main(void) {
   int sock;   /* fd for main socket */
   int msgsock;   /* fd from accept return */
   struct sockaddr_in server;   /* socket struct for server connection */
   struct sockaddr_in client;   /* socket struct for client connection */
   int clientLen;   /* returned length of client from accept() */
   int rval;   /* return value from read() */
   char buf[BUFFER_SIZE];   /* receive buffer */
   char connected_clients[5][2];
   int num_clients = 0;

   /* Open a socket, not bound yet.  Type is Internet TCP. */
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      pdie("Opening stream socket");

   /*
      Prepare to bind.  Permit Internet connections from any client
      to our SERVER_PORT.
   */
   bzero((char *) &server, sizeof(server));
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port = htons(SERVER_PORT);
   if (bind(sock, (struct sockaddr *) &server, sizeof(server)))
      pdie("Binding stream socket");

   printf("Socket has port %hu\n", ntohs(server.sin_port));

   /* Set the listen queue to 5, the maximum. */
   listen(sock, 5);

   /* Loop, waiting for client connections. */
   /* This is an interactive server. */
   while (TRUE) {

      clientLen = sizeof(client);
      if ((msgsock = accept(sock, (struct sockaddr *) &client,
                            &clientLen)) == -1){
         pdie("Accept");
		 }
      else {
         /* Print information about the client. */
         if (clientLen != sizeof(client))
            pdie("Accept overwrote sockaddr structure.");
		
		 printf("New Client connected.\n");
         printf("Client IP: %s\n", inet_ntoa(client.sin_addr));
         printf("Client Port: %hu\n", ntohs(client.sin_port));
		 /* Add client to our list */
		 connected_clients[num_clients][0] = inet_ntoa(client.sin_addr);
		 num_clients++;
		 printf("Users Connected: %d\n", num_clients);
		 

         do {   /* Read from client until it's closed the connection. */
            /* Prepare read buffer and read. */
            bzero(buf, sizeof(buf));
            if ((rval = read(msgsock, buf, BUFFER_SIZE)) < 0)
               pdie("Reading stream message");

            if (rval == 0)   /* Client has closed the connection */
               fprintf(stderr, "Ending connection\n");
            else
               printf("Client said: %s", buf);

            /* Write back to client. */
			/*
			write(msgsock, "client says: ", sizeof(14));
            if (write(msgsock, DATA, sizeof(DATA)) < 0)
               pdie("Writing on stream socket");
			*/
			/* Is this a command? Go find out and deal with it */
			if(buf[0] == '/'){
			  if(strncmp(buf, "/CONNECT", 7) == 0){
				printf("CONNECT cmd entered.\n");
			    cConnect(buf, inet_ntoa(client.sin_addr));
			  } else if(strncmp(buf, "/MSG", 4) == 0) {
				printf("MESSAGE cmd entered.\n");
			  } else if(strncmp(buf, "/WHO", 4) == 0) {
				printf("WHO cmd entered.\n");
			  } else if(strncmp(buf, "/JOIN", 5) == 0) {
				printf("JOIN cmd entered.\n");
			  } else if(strncmp(buf, "/PART", 5) == 0) {
				printf("PART cmd entered.\n");
			  } else if(strncmp(buf, "/QUIT", 5) == 0) {
				printf("QUIT cmd entered.\n");
				shutdown(msgsock, 2);
				cQuit(buf, inet_ntoa(client.sin_addr), &msgsock);
			  } else {
				char bad_command[] = "Invalid Command: ";
				printf("%s%s\n", bad_command, buf);
				write(msgsock, bad_command, sizeof(bad_command));
				write(msgsock, buf, sizeof(buf));
			  }
			}else{
			write(msgsock, buf, sizeof(buf));
			}

         } while (rval != 0);
      }   /* else */
      close(msgsock);
   }
   /*comment */
   exit(0);
}


/**********************************************************************
 * cQuit --- Deal with a QUIT command.
 **********************************************************************/
void cQuit(char *buf, char *address, int *msgsock){
	printf("this is where we kill the client\n");
	shutdown(&msgsock, 2);
}

/**********************************************************************
 * cConnect --- Deal with a CONNECT command. (Nickname)
 **********************************************************************/
void cConnect(char *buf, char *address){
	printf("this is where we add a nickname\n");
	printf("address incoming found is: %s with message:%s\n", address, buf);
	char *nick_name = &buf[9];
	printf("nick_name found is:%s\n", nick_name);	
}

/**********************************************************************
 * pdie --- Call perror() to figure out what's going on and die.
 **********************************************************************/
void pdie(const char *mesg) {
   perror(mesg);
   exit(1);
}


/**********************************************************************
 * die --- Print a message and die.
 **********************************************************************/
void die(const char *mesg) {
   fputs(mesg, stderr);
   fputc('\n', stderr);
   exit(1);
}
