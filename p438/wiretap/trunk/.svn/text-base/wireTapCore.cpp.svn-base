
#include <stdio.h>
#include <stdlib.h>
#include <string>//to support strings
#include <pcap.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <net/if_packet.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <netinet/ip6.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <map>
#include <math.h>
#include <utility>
#include <iostream>
#include <iomanip>
#include "pck_stats.h"
struct pck_stats cur_stats;

//#define ETHERTYPE_ARP 1544
//#define ETHERTYPE_IP 8


void chk_pack(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void show_pck_stats(void);
using namespace std;

struct	arp	{
	short	ar_hwtype;	/* hardware type			*/
	short	ar_prtype;	/* protocol type			*/
	//uint8_t	ar_hwlen;	/* hardware address length		*/
	//uint8_t	ar_prlen;	/* protocol address length		*/
	//short	ar_op;		/* ARP operation (see list above)	*/
	char	ar_sha[6];	/* sender's physical hardware address	*/
	struct in_addr	ar_spa;	/* sender's protocol address (IP addr.)	*/
	char	ar_ahd;
	char	ar_add;
	char	ar_tha[6];	/* target's physical hardware address	*/
	struct in_addr	ar_tpa;	/* target's protocol address (IP)	*/
};


pcap_t *get_capture(char *file_name){
//run pcap_open and pcap_loop
char errbuf[PCAP_ERRBUF_SIZE];
pcap_t *capture;
capture = pcap_open_offline(file_name, errbuf);
printf("Exiting get_capture.\n");
return capture;
}

void prnt_total_tm(){	
	char tmbuffer[30];
	strftime(tmbuffer,30,"%m-%d-%Y  %T.",localtime(&cur_stats.cap_start.tv_sec));
	printf("\nCapture Start at: %s%ld\n",tmbuffer,cur_stats.cap_start.tv_usec);	
	char tmbuffer2[30];
	strftime(tmbuffer2,30,"%m-%d-%Y  %T.",localtime(&cur_stats.prv_pck_tm.tv_sec));
	printf("Capture end at: %s%ld\n",tmbuffer2,cur_stats.prv_pck_tm.tv_usec);

	long int usec_st = cur_stats.cap_start.tv_usec;
	long int usec_end = cur_stats.prv_pck_tm.tv_usec;
	long int sec_st = cur_stats.cap_start.tv_sec;
	long int sec_end = cur_stats.prv_pck_tm.tv_sec;	
	
	long long double sec_d = sec_end - sec_st;
	long long double usec_d = usec_end - usec_st;
	if(sec_d == 0){
		usec_d = usec_d * 1000;
		cout<<"Total time: 0."<<setw(9)<<setfill('0')<<usec_d<<endl;
	}else{
		int num_digits = 0;
		int num = (int)(-1 * usec_d);
		while(num > 1){
			num_digits++;
			num/=10;
		}
		long long double t_num;
		if(num_digits + 3 < 9){
		t_num = (long long int)pow(10, num_digits+1);
		}else{
		t_num = (long long int)pow(10, num_digits);
		}
		t_num -= (-1 * usec_d);
		t_num = t_num * 1000;
		if(usec_end < usec_st){
			sec_d--;
		}
		cout<<"Total Time: "<<sec_d<<"."<<setw(9)<<setfill('0')<<t_num<<endl;
	}	
}

void show_pck_stats(void){
	map<string,int>::iterator iter;
	printf("\n*****Beginning packet statistics dump.*****\n");	
	printf("\t***START Ethernet Layer***\n");
	printf("\t\t--START Ethernet Layer Unique Sources--\n");
	for( iter = cur_stats.eth_uniq_src.begin(); iter != cur_stats.eth_uniq_src.end(); ++iter ) {
	  cout << "\t\t  MAC: " << iter->first << "   \tPck#: " << iter->second << endl;
	}
	printf("\t\t--END Ethernet Layer Unique Sources--\n");
	printf("\t\t--START Ethernet Layer Unique Destinations--\n");
	for( iter = cur_stats.eth_uniq_dst.begin(); iter != cur_stats.eth_uniq_dst.end(); ++iter ) {
	  cout << "\t\t  MAC: " << iter->first << "   \tPck#: " << iter->second << endl;
	}
	printf("\t\t--END Ethernet Layer Unique Destinations--\n");
	printf("\t***END Ethernet Layer***\n");
	printf("\t***START IP Layer***\n");
	printf("\t\t--START IP Layer Unique Sources--\n");
	for( iter = cur_stats.ip_uniq_src.begin(); iter != cur_stats.ip_uniq_src.end(); ++iter ) {
	  cout << "\t\t  IP: " << iter->first << " \tPck#: " << iter->second << endl;
	}
	printf("\t\t--END IP Layer Unique Sources--\n");
	printf("\t\t--START IP Layer Unique Destinations--\n");
	for( iter = cur_stats.ip_uniq_dst.begin(); iter != cur_stats.ip_uniq_dst.end(); ++iter ) {
	  cout << "\t\t  IP: " << iter->first << "   \tPck#: " << iter->second << endl;
	}	
	printf("\t\t--END IP Layer Unique Destinations--\n");
	printf("\t***END IP Layer***\n");
	printf("\t***START UDP Information***\n");
	printf("\t\t--START UDP Layer Unique Sources--\n");
	map<unsigned int,int>::iterator iter2;
 	for( iter2 = cur_stats.udp_uniq_src.begin(); iter2 != cur_stats.udp_uniq_src.end(); ++iter2 ) {
	  cout << "\t\t  UDPSrcPort: " << iter2->first << " \tPck#: " << iter2->second << endl;
	}
	printf("\t\t-END UDP Layer Unique Sources--\n");
	printf("\t\t-START UDP Layer Unique Destinations--\n");
	for( iter2 = cur_stats.udp_uniq_dst.begin(); iter2 != cur_stats.udp_uniq_dst.end(); ++iter2 ) {
	  cout << "\t\t  UDPDstPort: '" << iter2->first << " \tPck#: " << iter2->second << endl;
	}
	printf("\t\t-END UDP Layer Unique Destinations--\n");
	printf("\t***END UDP Layer***\n");
	printf("\t***START ARP Information***\n");
	printf("\t\t--START ARP Machines--\n");
	map<string,string>::iterator iter3;
	for( iter3 = cur_stats.arp_machines.begin(); iter3 != cur_stats.arp_machines.end(); ++iter3 ) {
	  cout << "\t\t  ARP IP: '\t" << iter3->first << "      \t\tPck#: " << iter3->second << endl;
	}
	printf("\t\t--END ARP Machines--\n");
	printf("\t***END ARP Information***\n");
	printf("\t!!TOTAL PACKETS READ:%d!!\n",cur_stats.total_packets);	
	printf("\t!!AVG PACKETS SIZE:%d!!\n",(cur_stats.total_size/cur_stats.total_packets));
	printf("\t!!MIN PACKET SIZE:%d!!\n",cur_stats.min_pck_sz);
	printf("\t!!MAX PACKET SIZE:%d!!\n",cur_stats.max_pck_sz);
	prnt_total_tm();
	printf("*****END packet statistics dump.*****\n");
}

void chk_pack(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
	static int count = 1; /*packet counter */
	static timeval init_tm = header->ts;
	static long int init_tm_us = header->ts.tv_usec;
	static struct pck_stats maps;
	cur_stats.cap_start = init_tm;
	timeval cur_tm = header->ts;
	
	/* declare pointers to packet headers */
	const struct ether_header *ethernet;  /* The ethernet header [1] */
	const struct ip *ip;                  /* The IP header */
	const struct tcp_header *tcp;		  /* The TCP header */
	const struct arp *arp;			      /* The ARP header */
	const char *payload;                  /* Packet payload */
	const struct udphdr *uptr;
	
	/* define ethernet header */
	ethernet = (struct ether_header *)(packet);
	ip = (struct ip *)(packet);
	tcp = (struct tcp_header *)(packet);
	arp = (struct arp *)(packet);
	uptr = (struct udphdr *)(packet);
	
	int offset = 0;
	int arp_proto = 0;
	int arp_send = 0;
	int arp_tar = 0;
	int arp_send1 = 0;
	int arp_tar1 = 0;
	int arp_send2 = 0;
	int arp_tar2 = 0;
	int arp_send3 = 0;
	int arp_tar3 = 0;
	
	char tmbuffer[30];
	char tm_chgbuffer[30];
	long int time_chg_us;
	if(count == 1){	
		time_chg_us = 0;
	}else{	
		time_chg_us = cur_tm.tv_usec - cur_stats.prv_pck_tm.tv_usec;
	}
	//set precision of tm_chg
	if(cur_stats.prv_pck_tm.tv_sec != cur_tm.tv_sec){
		time_chg_us = time_chg_us*-1;
		int num_digits = 0;
		long int num = time_chg_us;
		while(num > 0){
			num_digits++;
			num/=10;
		}
		long int t_num = (long int)pow(10,num_digits);
		time_chg_us = t_num - time_chg_us;
	}
	time_chg_us = time_chg_us *1000;
	cur_stats.prv_pck_tm = cur_tm;
	strftime(tmbuffer,30,"%m-%d-%Y  %T.",localtime(&cur_tm.tv_sec));
	printf("\n%d:  Packet recieved at: %s%ld\n",count,tmbuffer,header->ts.tv_usec);
	//add time since last frame
	cout << "Time since last packet: ."<< setw(9) << setfill('0') << time_chg_us <<  " seconds.\n";
	
	
	//update time vars
		string eth_dst = ether_ntoa((struct ether_addr *)ethernet->ether_shost);
		string eth_src = ether_ntoa((struct ether_addr *)ethernet->ether_dhost);
        printf("\tDestination: %s\n", eth_dst.c_str());
        printf("\tSource: %s\n", eth_src.c_str());
		//add to maps
 		if(cur_stats.eth_uniq_src.find(eth_src) != cur_stats.eth_uniq_src.end()){
			int val = cur_stats.eth_uniq_src[eth_src];
			cur_stats.eth_uniq_src[eth_src] = val + 1;			
		}else{
			// ot there
			cur_stats.eth_uniq_src.insert(make_pair(eth_src,1));
		}
		if(cur_stats.eth_uniq_dst.find(eth_dst) != cur_stats.eth_uniq_dst.end()){
			int val = cur_stats.eth_uniq_dst[eth_dst];
			cur_stats.eth_uniq_dst[eth_dst] = val + 1;	
		}else{
			// ot there
			cur_stats.eth_uniq_dst.insert(make_pair(eth_dst,1));
		} 
	printf("\tEther_type is:%d\n", ethernet->ether_type);
	offset += sizeof(struct ether_header);
	
	if(ethernet->ether_type == 1544){
		printf("\tARP protocol found.\n");
		arp = (struct arp *)(packet + (offset+2));
		/* print source and destination IP addresses */
		arp_proto = ntohs(arp->ar_prtype);
		printf("\tProtocol Type: %x\n", arp_proto);
/* 		arp_send = ntohs(arp->ar_spa[0]);
		arp_tar = ntohs(arp->ar_tpa[0]);
		arp_send1 = ntohs(arp->ar_spa[1]);
		arp_tar1 = ntohs(arp->ar_tpa[1]);
		arp_send2 = ntohs(arp->ar_spa[2]);
		arp_tar2 = ntohs(arp->ar_tpa[2]);
		arp_send3 = ntohs(arp->ar_spa[3]);
		arp_tar3 = ntohs(arp->ar_tpa[3]);
		printf("\tIP From: %x.", arp_send);
		printf("%x.", arp_send1);
		printf("%x.", arp_send2);
		printf("%x\n", arp_send3);
		printf("\tIP To: %x.", arp_tar);
		printf("%x.", arp_tar1);
		printf("%x.", arp_tar2);
		printf("%x\n", arp_tar3); */
		//printf("\n\t\t\tThis is a test\n");
		//cout << arp->ar_hwlen << " " << arp->ar_prlen << arp->ar_op << endl;
		//printf("ar_hwlen: %c, ar_prlen: %c, ar_op: %x\n", arp->ar_hwlen, arp->ar_prlen, arp->ar_op);
		
		printf("\tTest Src: %s\n",inet_ntoa(arp->ar_spa));
		printf("\tTest Dst: %s\n",inet_ntoa(arp->ar_tpa));
		//add to maps
		//string arp_dst = inet_ntoa((in_addr *)(arp->ar_sha));
		string arp_src = inet_ntoa(arp->ar_spa);
		if(cur_stats.arp_machines.find(inet_ntoa(arp->ar_spa)) != cur_stats.arp_machines.end()){
			//nothing
		}else{
			// ot there
			cur_stats.arp_machines.insert(make_pair(arp_src,eth_dst));
		}
		
	}else if(ethernet->ether_type == 8){
		printf("\tIP protocol found\n");
		ip = (struct ip *)(packet + offset);
		/* print source and destination IP addresses */
		string ip_src = inet_ntoa(ip->ip_src);
		string ip_dst = inet_ntoa(ip->ip_dst);
		printf("\tFrom: %s\n", ip_src.c_str());
		printf("\tTo: %s\n", ip_dst.c_str());
		//add to maps
 		if(cur_stats.ip_uniq_src.find(ip_src) != cur_stats.ip_uniq_src.end()){
			int val = cur_stats.ip_uniq_src[ip_src];
			cur_stats.ip_uniq_src[ip_src] = val + 1;	
		}else{
			// ot there
			cur_stats.ip_uniq_src.insert(make_pair(ip_src,1));
		}
		if(cur_stats.ip_uniq_dst.find(ip_dst) != cur_stats.ip_uniq_dst.end()){
			int val = cur_stats.ip_uniq_dst[ip_dst];
			cur_stats.ip_uniq_dst[ip_dst] = val + 1;	
		}else{
			// ot there
			cur_stats.ip_uniq_dst.insert(make_pair(ip_dst,1));
		} 
		
		//check for ipv6
		if(ip->ip_hl > 5){
			//we have ipv6
			printf("\nWe have invalid IP header length. Now exiting.\n");
			exit(1);
		}else{
			offset += sizeof(struct ip);	
		}
		/* determine protocol */
		switch(ip->ip_p) {
			case 17:
				printf("\n\tProtocol: UDP\n");
				uptr = (struct udphdr *)(packet + offset);
				printf("\tFrom: %u \n\t  To: %u \n", ntohs(uptr->source), ntohs(uptr->dest));
				printf("\tLength: %u\n", ntohs(uptr->len));
				//add to maps
				if(cur_stats.udp_uniq_src.find(ntohs(uptr->source)) != cur_stats.udp_uniq_src.end()){
					int val = cur_stats.udp_uniq_src[ntohs(uptr->source)];
					cur_stats.udp_uniq_src[ntohs(uptr->source)] = val + 1;
				}else{
					// ot there
					cur_stats.udp_uniq_src.insert(make_pair(ntohs(uptr->source),1));
				}
				if(cur_stats.udp_uniq_dst.find(ntohs(uptr->dest)) != cur_stats.udp_uniq_dst.end()){
					int val = cur_stats.udp_uniq_dst[ntohs(uptr->dest)];
					cur_stats.udp_uniq_dst[ntohs(uptr->dest)] = val + 1;
				}else{
					// ot there
					cur_stats.udp_uniq_dst.insert(make_pair(ntohs(uptr->dest),1));
				} 
				break;
			default:
				printf("\tProtocol: Non UDP\n");
				break;
		}
	}else{
		printf("Houston we have a problem.\n");
	}
	
	if(cur_stats.min_pck_sz == 0){
		cur_stats.min_pck_sz = -1;
	}
	if(cur_stats.total_size == 0) cur_stats.total_size = 0;
	if(cur_stats.max_pck_sz == 0)cur_stats.max_pck_sz = 0;
	cur_stats.total_packets = count++;
	int cur_pck_sz = header->len;
	cur_stats.total_size += cur_pck_sz;
	if(cur_pck_sz < cur_stats.min_pck_sz || cur_stats.min_pck_sz == -1){
		cur_stats.min_pck_sz = cur_pck_sz;
	}
	if(cur_pck_sz > cur_stats.max_pck_sz){
		cur_stats.max_pck_sz = cur_pck_sz;
	}
	
}

int main(int argc, char* argv[]){
	char *file_name = argv[1];
	int link_type = -1;
	int loop_ret;
	printf("file name found:%s\n",file_name);
	pcap_t *capture;
	capture = get_capture(file_name);
	link_type = pcap_datalink(capture);	
	if(link_type == 1){
		printf("Link type found:%d\n",link_type);
		//continue
		loop_ret = pcap_loop(capture, 0, chk_pack, NULL);
	}else{		
		printf("File is not of type Ethernet.Aborting\n");
		exit(1);
	}
	
	show_pck_stats();

	printf("Exiting main.\n");
	pcap_close(capture);
	return 0;
}


