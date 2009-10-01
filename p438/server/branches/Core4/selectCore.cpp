/*
** selectserver.c -- a cheezy multiperson chat server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <cstring>
#include "ircClients.cpp" //our client information struct

using namespace std;


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
	//first try IPv4
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
	//we are here only if IPv4 didn't work, so try IPv6
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void send_private_msg(int fd, char *buf, int nbytes){
	if (send(fd, buf, nbytes, 0) == -1) {
		perror("send");
	}
}

string get_client_prefix(int fd, vector<clientData> &connected_clients){
	printf("Now scanning vector for client nick on socket:%d\n",fd);
	int client_vec_size = connected_clients.size();
	int id_match = client_vec_size + 1;
	for(int i = 0; i < client_vec_size;i++){
		if(connected_clients[i].client_fd == fd){
			id_match = i;
			break;
		}
	}
	return connected_clients[id_match].nickname;
}

//take a fd, return clients index in the connected_clients vector
int get_client_position(int fd, vector<clientData> &connected_clients){
	printf("Now scanning vector for client info on socket:%d\n",fd);
	int client_vec_size = connected_clients.size();
	for(int i = 0; i < client_vec_size;i++){
		if(connected_clients[i].client_fd == fd){
			return i;
		}
	}
}

int get_client_fd(string nickname, vector<clientData> &connected_clients){
	printf("Now scanning vector for client fd with nick:%s\n",nickname.c_str());
	int client_vec_size = connected_clients.size();
	for(int i = 0; i < client_vec_size;i++){
		string cc_nick(connected_clients[i].nickname);
		int cc_n_len = cc_nick.length();
		cc_nick.erase(cc_n_len - 2);
		printf("nickname:'%s' (size:%d len:%d) vs:'%s' (size:%d len:%d)\n",
		nickname.c_str(), nickname.size(),nickname.length(),
		cc_nick.c_str(),cc_nick.size(),
		cc_nick.length());
		if(nickname.compare(cc_nick) == 0){
			return connected_clients[i].client_fd;
		}
	}
	return -1;
}

void cConnect(char *buf,int fd,vector<clientData> &connected_clients){
	string nick_request = &buf[9];
	int id_match;
	bool nick_usable = true;
	for(int i = 0; i < connected_clients.size();i++){
		if(connected_clients[i].client_fd == fd){
			id_match = i;
		}
		if(strcmp(connected_clients[i].nickname.c_str(), nick_request.c_str()) ==0){
			printf("The nickname requested is already in use.\n");
			nick_usable = false;
		}
	}
	//check for existing nickname
	if(nick_usable){
	int length_nick = strlen(nick_request.c_str());
	string use_this;
	nick_request.substr(0,length_nick-2);
	connected_clients[id_match].nickname = nick_request;
	connected_clients[id_match].registered = true;
	printf("nick_name changed to:%s\n", connected_clients[id_match].nickname.c_str());
	}else{
		string nick_used = "Nick name requested is already in use. Please try again.\n";
		int nick_used_size = nick_used.size();
		printf("nick_name requested:%s is already in use. Did NOT update.\n",nick_request.c_str());
		send(fd, nick_used.c_str(), nick_used_size,0);
	}
}

void cMsg(char *buf, int fd, vector<clientData> &connected_clients){
	//parse args
	string recieve = &buf[5];
	int find_ofindex = recieve.find_first_of(' ',0);
	char *message = &recieve[find_ofindex];
	string target =	recieve.substr(0,find_ofindex);
	printf("result found...target:%s and message:%s\n",target.c_str(),message);
	int target_fd = get_client_fd(target, connected_clients);
	printf("target_fd returned as:%d\n",target_fd);
	if(target_fd != -1){
		//send pm
		int str_size = strlen(message);
		send_private_msg(target_fd, message, str_size);
	}else{
		//no user
		string bad_target = "No user found with that nickname. Pleas try again.\n";
		printf("No user found for requested /MSG call. Nothing happened.\n");
		//send_private_msg(fd, bad_target.c_str(),bad_target.size());
	}
}

void send_message(int fd, fd_set &master, int fdmax, int listener, char *buf, vector<clientData> &connected_clients){
//TODO		
//who is sending?
string name = get_client_prefix(fd, connected_clients);
int namesize = name.size();
int nbytes = strlen(buf);
for(int j = 0; j <= fdmax; j++) {
	//broadcast this data
	if (FD_ISSET(j, &master)) {
		//this will NOT echo back to sender						
		if (j != listener && j != fd) { //if (j != listener && j != i) {
			//call output function
			send(j, name.c_str(), namesize, 0);
			send_private_msg(j, buf, nbytes);
		}
	}
}
//write to server log (do once)
//TODO: change to logging
}


//void handle_command(string buf, int i);
void handle_command(char *buf, int fd, vector<clientData> &connected_clients, fd_set &read_fds, fd_set &master){
  if(strncmp(buf, "/CONNECT", 8) == 0){
	printf("CONNECT cmd entered:%s\n",buf);
	cConnect(buf,fd,connected_clients);
  } else if(strncmp(buf, "/MSG", 4) == 0) {
	printf("MESSAGE cmd entered:%s\n",buf);
	cMsg(buf, fd, connected_clients);
  } else if(strncmp(buf, "/WHO", 4) == 0) {
	printf("WHO cmd entered:%s\n",buf);
  } else if(strncmp(buf, "/JOIN", 5) == 0) {
	printf("JOIN cmd entered:%s\n",buf);
  } else if(strncmp(buf, "/PART", 5) == 0) {
	printf("PART cmd entered:%s\n",buf);
  } else if(strncmp(buf, "/QUIT", 5) == 0) {
	printf("QUIT cmd entered:%s\n",buf);
	int x = get_client_position(fd, connected_clients);
	printf("conn_clients size before removal:%d\n",connected_clients.size());
	connected_clients.erase(connected_clients.begin() + x);
	printf("conn_clients size after removal:%d\n",connected_clients.size());	
	FD_CLR(fd, &master);
	FD_CLR(fd, &read_fds);
	if(close(fd) != 0){
		printf("Error trying to close sockfd:%d",fd);
	}else{
		printf("Closed sockfd:%d",fd);
	}
  } else {
	char bad_command[] = "Invalid Command: ";
	printf("%s%s\n", bad_command, buf);
  }
}

int main(int argc, char* argv[]){
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
	clientData temp_client;
	vector<clientData> connected_clients;

    char buf[256];    // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1;        //for funsies.
    int i, j, rv;

    struct addrinfo hints, *ai, *p; //hold server prefs
	
	char *PORT = "5991";
	char *RATENUM = "4";
	char *RATETM = "60";
	char *MAX_USERS = "3";
	
	//grab cmd line args and set them as vars
	printf("Cmd line args found:%d\n",argc);
	if(argc < 5){
		printf("Problem reading cmd line arguments. Continuing with defaults.\n");
	}else if(argc == 5){
		//use args
		PORT = argv[1];
		RATENUM = argv[2];
		RATETM = argv[3];
		MAX_USERS = argv[4];		
	}else{
		printf("Problem reading cmd line arguments. Continuing with defaults.\n");
	}
	printf("Beginning boot sequence with params...\nPORT:%s\nRATENUM:%s RATETM:%s\nMAX_USERS:%s\n",PORT,RATENUM,RATETM,MAX_USERS);	

    FD_ZERO(&master);    // 0 out the socket sets
    FD_ZERO(&read_fds);

    //set socket prefs
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
	//try to get addr information
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { 
            continue;
        }
        
        //set address to re-usable...YAY!
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		//try to bind our listener 
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }

    // verify we got server to bind
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); //make sure to free mem
	
	//We are ready to listen
	printf("Server core boot success, listening on port:%s\n", PORT);

    // try to listen with our listener sck
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }
	
    // add the current listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; 

    // main loop
    for(;;) {
        read_fds = master; //set temp set to current list from master
		
		//try to grab a socket from the set
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        //loop through sck list looking for I/O request
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { //true if I/O request is there
                if (i == listener) { //we have a new connection, deal with it
                    // handle the new connections
                    addrlen = sizeof remoteaddr;
					
					//check for MAX_USER limit
					int cur_users = connected_clients.size();
					int user_limit = atoi(MAX_USERS);
					if(cur_users >= user_limit){
						//deny access
						printf("Client attempted to connect, but we are already at user limit. Access denied.\n");
						int tempfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);						
						string server_full = "The server is currently full. Please try again later.\n";
						int svfull_size = server_full.size();
						send(tempfd, server_full.c_str(), svfull_size, 0);
						close(tempfd);
						break;
					}
					
					//accept the fd
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr,
                        &addrlen);
                    if (newfd == -1) { //make sure we accepted without problems
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); //add to master set
                        if (newfd > fdmax) {    //make sure to update max
                            fdmax = newfd;
                        }
						//add this client to our clientData struct
						string current_ip = inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN);
						temp_client.address = current_ip;
						temp_client.client_fd = newfd;
						//set nickname to be address, later update nickname
						temp_client.nickname = current_ip;
						temp_client.registered = false;
						temp_client.client_addr = remoteaddr;						
						connected_clients.push_back(temp_client);
						//show some info on new connection
                        printf("ircCore: New Client from %s on "
                            "socket %d. Added to vector.\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                    }
                } else { //true if we have an I/O request from a connected client
					//try to recv the data into our buffer
					bzero(&buf, sizeof(buf));
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        if (nbytes == 0) {
                            //the connection ended remotely
                            printf("ircCore: Client disconnected. Socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); //close gracefully
                        FD_CLR(i, &master); //update master fd set
                    } else {
                        //data from client is usable		
						//this is where we intercept COMMANDS
						if(buf[0] == '/'){
							//go off and handle
							if(strncmp(buf, "/CONNECT", 8) == 0){
							//set nick and register
							cConnect(buf,i,connected_clients);
							}else{
								//check for register
								int loc = get_client_position(i, connected_clients);
									if(connected_clients[loc].registered){
										handle_command(buf, i, connected_clients, read_fds, master);
									}else{
										//deny
										string not_connected = "Command Denied. Please register (CONNECT) before doing anything else.";
										int nc_size = not_connected.size();
										send(i, not_connected.c_str(), nc_size, 0);
									}
							continue;
							}
						}
						//will be changed
						send_message(i, master, fdmax, listener, buf, connected_clients);
					}
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)
    
    return 0;
}


