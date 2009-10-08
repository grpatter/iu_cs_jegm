#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "parse_config_file.hpp"

using namespace std;

bool search_routers(string real_ip){
  bool is_router = false;
  vector<Routers>::iterator ri;
  for(ri = r_inst.begin(); ri < r_inst.end(); ri++) {
    if(real_ip == ri->ext_ip) {
      is_router = true;
    }
  }
  return is_router;
}

bool search_end_hosts(string real_ip){
  bool is_endhost = false;
  vector<EndHosts>::iterator hi;
  for(hi = h_inst.begin(); hi < h_inst.end(); hi++) {
    cout << hi->eh_id << " " << hi->ext_ip << " " << hi->vir_ip <<endl;
    if(real_ip == hi->ext_ip) {
      is_endhost = true;
    }
  }
  return is_endhost;
}

