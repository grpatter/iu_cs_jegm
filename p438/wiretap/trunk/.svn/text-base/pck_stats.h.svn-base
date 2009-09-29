#include <map>
#include <string>//to support strings
using namespace std;
struct pck_stats{
	map <string,int> eth_uniq_src;
	map <string,int> eth_uniq_dst;
	map <string,int> ip_uniq_src;
	map <string,int> ip_uniq_dst;
	map <unsigned int,int> udp_uniq_src;
	map <unsigned int,int> udp_uniq_dst;
	map <string,string> arp_machines;
	int total_packets;
	int total_size;
	int min_pck_sz;
	int max_pck_sz;
	timeval cap_start;
	timeval prv_pck_tm;
};

extern struct pck_stats cur_stats;
