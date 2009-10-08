#include <stdlib.h>
#include <string.h>

/*udp header struct*/
struct udp_struct{
  int src_port;
  int dest_port;
};

/*virtual packet header struct*/
struct vir_head{
  char *data;
  struct udp_struct udphd;
  string ip4_str;
};

