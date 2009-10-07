/**********************************************************************
 * Data structure to hold information about clients using the server.
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;
// Define a struct to hold each client data
struct clientData {
    string address;   // 
    int client_fd;   // 
    string  nickname; //
	bool registered;
	vector<string> channels;
	struct sockaddr_storage client_addr;
};
