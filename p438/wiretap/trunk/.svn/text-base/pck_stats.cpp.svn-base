#include <map>
#include <string>//to support strings
#include "pck_stats.h"
using namespace std;
void show_pck_stats(void){
	struct pck_stats *ptr_stats;
	*ptr_stats = &cur_stats;
	map<string,int>::iterator iter;
	printf("\n*****Beginning packet statistics dump.*****\n");
	for( iter = cur_stats->eth_uniq_src.begin(); iter != cur_stats->eth_uniq_src.end(); ++iter ) {
	  cout << "ESrcKey: '" << iter->first << "', ESrcValue: " << iter->second << endl;
	}
	for( iter = cur_stats->eth_uniq_dst.begin(); iter != cur_stats->eth_uniq_dst.end(); ++iter ) {
	  cout << "EDstKey: '" << iter->first << "', EDstValue: " << iter->second << endl;
	}
	for( iter = cur_stats->ip_uniq_src.begin(); iter != cur_stats->ip_uniq_src.end(); ++iter ) {
	  cout << "IPSrcKey: '" << iter->first << "', IPSrcValue: " << iter->second << endl;
	}
	for( iter = cur_stats->ip_uniq_dst.begin(); iter != cur_stats->ip_uniq_dst.end(); ++iter ) {
	  cout << "IPDstKey: '" << iter->first << "', IPDstValue: " << iter->second << endl;
	}	
	map<int,int>::iterator iter2;
	for( iter2 = cur_stats->udp_uniq_src.begin(); iter != cur_stats->udp_uniq_src.end(); ++iter2 ) {
	  cout << "UDPSrcKey: '" << iter->first << "', UDPSrcValue: " << iter->second << endl;
	}
	for( iter2 = cur_stats->udp_uniq_dst.begin(); iter != cur_stats->udp_uniq_dst.end(); ++iter2 ) {
	  cout << "UDPDstKey: '" << iter->first << "', UDPDstValue: " << iter->second << endl;
	}	
	map<string,string>::iterator iter3;
	for( iter3 = cur_stats->arp_machines.begin(); iter != cur_stats->arp_machines.end(); ++iter3 ) {
	  cout << "ARPKey: '" << iter->first << "', ARPValue: " << iter->second << endl;
	}
	printf("\n*****END packet statistics dump.*****\n");
}
