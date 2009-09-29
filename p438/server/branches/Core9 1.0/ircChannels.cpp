/**********************************************************************
 * Data structure to hold information about channels on the server.
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;
// Define a struct to hold each channels data
struct channelData {
    string name;   // to hold channel name
	bool active; //for funsies
	vector<int> members; //to hold sockfd of all clients in the channel
};
