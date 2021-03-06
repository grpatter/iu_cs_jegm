
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
#include <iostream>
#include <sstream>
#include <time.h>
#include "ircClients.cpp" //our client information struct
#include "ircChannels.cpp" //our channel info struct

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

string get_client_prefix(int fd, vector<clientData> &connected_clients){
  printf("\tNow scanning vector for client nick on socket:%d\n",fd);
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
  printf("\tNow scanning vector for client info on socket:%d\n",fd);
  int client_vec_size = connected_clients.size();
  for(int i = 0; i < client_vec_size;i++){
    if(connected_clients[i].client_fd == fd){
      return i;
    }
  }
  return -1;
}

int get_client_fd(string nickname, vector<clientData> &connected_clients){
  printf("\tNow scanning vector for client fd with nick: %s\n",nickname.c_str());
  int client_vec_size = connected_clients.size();
  for(int i = 0; i < client_vec_size;i++){
    string cc_nick(connected_clients[i].nickname);
    //int cc_n_len = cc_nick.length();
    //cc_nick.erase(cc_n_len - 2);
    printf("\tnickname:'%s' (size:%d len:%d) vs:'%s' (size:%d len:%d)\n",
	   nickname.c_str(), nickname.size(),nickname.length(),
	   cc_nick.c_str(),cc_nick.size(),
	   cc_nick.length());
    if(nickname.compare(cc_nick) == 0){
      return connected_clients[i].client_fd;
    }
  }
  return -1;
}

bool argsCorrect(int expect, stringstream &ss){
	string temp;
	vector<string> tokens;
	while(ss >> temp){
		tokens.push_back(temp);
	}
	int tkn_sz = tokens.size();
	printf("Args founds:%d\n",tkn_sz);
	if(tkn_sz != expect){
		printf("Invalid arguments for %s cmd.\n",tokens[0].c_str());
		return false;
	}else{
		return true;
	}
}

void cConnect(char *buf,int fd,vector<clientData> &connected_clients){
	//verify using stringstreams
	stringstream ss(buf);
	if(!argsCorrect(2,ss)){
		printf("Invalid arguments for CONNECT cmd.\n");
		return;
	}
	//
  string nick_request = &buf[9];
  string use_nick(nick_request);
  int nick_len = use_nick.length();
  	if(nick_len < 2){
		use_nick += "  ";
		nick_len = use_nick.length();
	}
  use_nick.erase(nick_len - 2);
  int id_match;
  bool nick_usable = true;
  for(int i = 0; i < connected_clients.size();i++){
    if(connected_clients[i].client_fd == fd){
      id_match = i;
    }
    if(strcmp(connected_clients[i].nickname.c_str(), use_nick.c_str()) ==0){
      nick_usable = false;
    }
  }
  //check for existing nickname
  if(nick_usable){
    connected_clients[id_match].nickname = use_nick;
    connected_clients[id_match].registered = true;
    printf("clients nick_name has been updated to %s.\n", connected_clients[id_match].nickname.c_str());
  }else{
    string nick_used = "The nickname is already in use. Please try a different nickname.\n";
    int nick_used_size = nick_used.size();
    printf("The nickname (%s) is already in use. The nickname was NOT updated.\n",use_nick.c_str());
    send(fd, nick_used.c_str(), nick_used_size,0);
  }
}

void cMsg(char *buf, int fd, vector<clientData> &connected_clients, vector<channelData> &current_channels){
  //parse args
  	//verify using stringstreams
	stringstream ss(buf);
	if(!argsCorrect(3,ss)){
		printf("Invalid arguments for MSG cmd.\n");
		return;
	}
	//
  string recieve = &buf[5];
  int find_ofindex = recieve.find_first_of(' ',0);
  char *message = &recieve[find_ofindex];
  string target = recieve.substr(0,find_ofindex);
  if(target[0] == '#'){
    target = &target[1];
    string send_this = "MSG ";
    send_this += get_client_prefix(fd,connected_clients);
    send_this += " ";
    send_this += target;
    send_this += " ";
    send_this += message;
    send_this += "\n";
    cout<<"\tSending:"<<send_this;
    int send_this_sz = send_this.size();		
    int cur_channel_sz = current_channels.size();
    for(int i = 0; i < cur_channel_sz; i++){			
      if(target.compare(current_channels[i].name) == 0){
	int mem_sz = current_channels[i].members.size();
	for(int y = 0;y<mem_sz;y++){
	  send(current_channels[i].members[y], send_this.c_str(), send_this_sz, 0);
	}
      }
    }
  }else{
    printf(" %s received: %s \n",target.c_str(), message);
    int target_fd = get_client_fd(target, connected_clients);
    printf("\ttarget socket found is%d\n",target_fd);
    if(target_fd != -1){
      //send pm
      string private_msg = get_client_prefix(fd, connected_clients);
      private_msg += ":";
      private_msg += message;
      int private_msg_len = private_msg.size();
      send(target_fd, private_msg.c_str(), private_msg_len, 0);
	/*
	  int str_size = strlen(message);
	  send_private_msg(target_fd, message, str_size);
	*/
    }else{
      //no user
      string bad_target = "Unknown receiver, message was not delivered. Please try again.\n";
      int bad_target_size = bad_target.size();
      printf("No user found for requested /MSG call. Nothing happened.\n");
      send(fd, bad_target.c_str(), bad_target_size, 0);
    }
  }
}

void cWho(char *buf, int fd, vector<clientData> &connected_clients, vector<channelData> &current_channels){
  //parse
    	//verify using stringstreams
	stringstream ss(buf);
	if(!argsCorrect(2,ss)){
		printf("Invalid arguments for WHO cmd.\n");
		return;
	}
	//
  string recieve = &buf[5];
  string cc_name(recieve);
  int cc_name_len = cc_name.length();
  cc_name.erase(cc_name_len - 2);
  printf("\tChannel name found:%s\n",cc_name.c_str());
  
  int cur_channel_sz = current_channels.size();
  for(int i = 0; i < cur_channel_sz; i++){
    if(cc_name.compare(current_channels[i].name) == 0){
      //check member vector
      int mem_vec_sz = current_channels[i].members.size();
      for(int x = 0; x < mem_vec_sz; x++){			
	int loc = get_client_position(current_channels[i].members[x], connected_clients);	
	string cur_mem_str = "WHO ";
	cur_mem_str += cc_name;
	cur_mem_str += " ";
	cur_mem_str += connected_clients[loc].nickname;
	cur_mem_str += " ";
	cur_mem_str += connected_clients[loc].address;
	cur_mem_str += "\n";
	int cur_mem_str_sz = cur_mem_str.size();
	send(fd, cur_mem_str.c_str(), cur_mem_str_sz, 0);
	cout << "\t" << cur_mem_str;
      }
      break;
    }
  }
}

void cJoin(char *buf, int fd, vector<clientData> &connected_clients, vector<channelData> &current_channels){
  //parse  	
	//verify using stringstreams
	stringstream ss(buf);
	if(!argsCorrect(2,ss)){
		printf("Invalid arguments for JOIN cmd.\n");
		return;
	}
	//
  string recieve = &buf[6];
  string cc_name(recieve);
  int cc_name_len = cc_name.length();
  cc_name.erase(cc_name_len - 2);
  printf("\tChannel name requested is %s\n",cc_name.c_str());
  //check if channel exists
  int cur_channel_sz = current_channels.size();
  bool exists = false;
  for(int i = 0; i < cur_channel_sz; i++){
    if(cc_name.compare(current_channels[i].name) == 0){
      //add them
      current_channels[i].members.push_back(fd);
      current_channels[i].active = true;
      exists = true;
      printf("\tChannel already exist, added client to channel.\n");
      break;
    }
  }
  if(!exists){
    //create and add
    channelData temp_chan;
    temp_chan.name = cc_name;
    temp_chan.active = true;
    temp_chan.members.push_back(fd);
    current_channels.push_back(temp_chan);
    printf("\tCreated channel: %s\n",temp_chan.name.c_str());
  }
  int loc = get_client_position(fd, connected_clients);
  connected_clients[loc].channels.push_back(cc_name);
}

void cPart(char *buf, int fd, vector<clientData> &connected_clients, vector<channelData> &current_channels){
  //parse
    	//verify using stringstreams
	stringstream ss(buf);
	if(!argsCorrect(2,ss)){
		printf("Invalid arguments for PART cmd.\n");
		return;
	}
	//
  string recieve = &buf[6];
  string cc_name(recieve);
  int cc_name_len = cc_name.length();
  cc_name.erase(cc_name_len - 2);
  printf("\tChannel name found: %s\n",cc_name.c_str());
  //check if channel exists
  int cur_channel_sz = current_channels.size();
  for(int i = 0; i < cur_channel_sz; i++){
    if(cc_name.compare(current_channels[i].name) == 0){
      //remove them
      int mem_vec_sz = current_channels[i].members.size();
      for(int x = 0;x<mem_vec_sz;x++){
		if(fd == current_channels[i].members[x]){
		  //remove
		  current_channels[i].members.erase(current_channels[i].members.begin() + x);
		  //print removal notifications
		  string rem_str = "You have been removed from";
		  string current_user = get_client_prefix(fd, connected_clients);
		  printf("\t%s has parted from %s.\n", current_user.c_str(), cc_name.c_str());
		  rem_str += current_channels[i].name;
		  rem_str += ".\n";
		  int rem_sz = rem_str.size();
		  send(fd, rem_str.c_str(), rem_sz, 0);				
		  break;
		}
      }
      //alert the rest of the channel					
      string clt_left = "PART ";
      clt_left += current_channels[i].name;
      clt_left += " ";
      clt_left += get_client_prefix(fd, connected_clients);
      clt_left += ".\n";
      int clt_left_sz = clt_left.size();
      int mem_sz = current_channels[i].members.size();
      for(int y = 0;y<mem_sz;y++){
		send(current_channels[i].members[y], clt_left.c_str(), clt_left_sz, 0);
      }
      break;
    }
  }
}

void forcePart(char *buf, int fd, vector<clientData> &connected_clients, vector<channelData> &current_channels, fd_set master, fd_set read_fds){
   int chan_vec_sz = current_channels.size();
   for(int i = 0; i < chan_vec_sz; i++){
		int cur_chan_sz = current_channels[i].members.size();
		for(int x = 0; x < cur_chan_sz; x++){
			if(fd == current_channels[i].members[x]){
				//they are here, remove them.
				string part_str = "/PART ";
				part_str += current_channels[i].name;
				part_str += "  ";
				printf("part_str len:%d\n",part_str.length());
				char buft[part_str.length()];
				strcpy(buft, part_str.c_str());
				printf("Generating part for user:%s\n",buft);
				cPart(buft, fd, connected_clients, current_channels);
			}
		}
   }
}

//void handle_command(string buf, int i);
void handle_command(char *buf, int fd, vector<clientData> &connected_clients, fd_set &read_fds, fd_set &master, vector<channelData> &current_channels, int rate_tm, int rate_num){
  string current_user = get_client_prefix(fd, connected_clients);
  int loc = get_client_position(fd, connected_clients);
  time_t req_tm=time(NULL);
  printf("Cmd requested at:%d\n",req_tm);
  //verify user is not exceeding rate limit
  bool allow_cmd = false;
  for(int i= 0; i < rate_num; i++){
	if(req_tm - connected_clients[loc].limits[i] >= rate_tm){
		connected_clients[loc].limits[i] = req_tm;
		allow_cmd = true;
		break;
	}
  }
  if(allow_cmd == false){
	printf("Cmd denied due to rate limiting.\n");
	return;
  }
  //
  if(strncmp(buf, "/CONNECT", 8) == 0){
    printf("%s: CONNECT cmd entered.\n",current_user.c_str());
    cConnect(buf,fd,connected_clients);
  } else if(strncmp(buf, "/MSG", 4) == 0) {
    printf("%s: MESSAGE cmd entered.\n",current_user.c_str());
    cMsg(buf, fd, connected_clients, current_channels);
  } else if(strncmp(buf, "/WHO", 4) == 0) {
    printf("%s: WHO cmd entered.\n",current_user.c_str());
    cWho(buf,fd,connected_clients,current_channels);
  } else if(strncmp(buf, "/JOIN", 5) == 0) {
    printf("%s: JOIN cmd entered.\n",current_user.c_str());
    cJoin(buf, fd, connected_clients, current_channels);
  } else if(strncmp(buf, "/PART", 5) == 0) {
    printf("%s: PART cmd entered.\n",current_user.c_str());
    cPart(buf, fd, connected_clients, current_channels);
  } else if(strncmp(buf, "/QUIT", 5) == 0) {
	forcePart(buf, fd, connected_clients, current_channels, master, read_fds);
	printf("%s: QUIT cmd entered.\n",current_user.c_str());
    int x = get_client_position(fd, connected_clients);
    printf("conn_clients size before removal: %d\n",connected_clients.size());
    connected_clients.erase(connected_clients.begin() + x);
    printf("conn_clients size after removal: %d\n",connected_clients.size());	
    FD_CLR(fd, &master);
    FD_CLR(fd, &read_fds);
	//force client out of all channels
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
  vector<channelData> current_channels;
  
  char buf[512];    // buffer for client data
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
		  vector<int> tmp_lm (100,0);
		  temp_client.limits = tmp_lm;
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
		    handle_command(buf, i, connected_clients, read_fds, master, current_channels, atoi(RATETM), atoi(RATENUM));
		  }else{
		    //deny
		    string not_connected = "Command Denied. Please register (CONNECT) before doing anything else.";
		    int nc_size = not_connected.size();
		    send(i, not_connected.c_str(), nc_size, 0);
		  }
		  continue;
		}
	      }
	      if(buf[0] != '/'){
		string not_connected = "Unreadable Text. Please sign in using (CONNECT).\n";
		int nc_size = not_connected.size();
		send(i, not_connected.c_str(), nc_size, 0);
	      }
	    }
	  } // END handle data from client
	} // END got new incoming connection
      } // END looping through file descriptors
    } // END for(;;)
    
    return 0;
}


