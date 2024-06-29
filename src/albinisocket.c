#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <netinet/in.h>


int ConexaoRawSocket(char *device)
{
  int soquete;
  struct ifreq ir;
  struct sockaddr_ll endereco;
  struct packet_mreq mr;

  soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  /* Create socket */
  if (soquete == -1) {
    perror("Socket error");
    return -1;
  }

  memset(&ir, 0, sizeof(struct ifreq));  /* Device eth0 */
  strncpy(ir.ifr_name, device, IFNAMSIZ-1);
  ir.ifr_name[IFNAMSIZ-1] = '\0';  /* Ensure null termination */
  if (ioctl(soquete, SIOCGIFINDEX, &ir) == -1) {
    perror("Ioctl error");
    close(soquete);
    return -1;
  }
  

  memset(&endereco, 0, sizeof(endereco));  /* IP of the device */
  endereco.sll_family = AF_PACKET;
  endereco.sll_protocol = htons(ETH_P_ALL);
  endereco.sll_ifindex = ir.ifr_ifindex;
  if (bind(soquete, (struct sockaddr *)&endereco, sizeof(endereco)) == -1) {
    perror("Bind error");
    close(soquete);
    return -1;
  }

  memset(&mr, 0, sizeof(mr));  /* Promiscuous mode */
  mr.mr_ifindex = ir.ifr_ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  if (setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
    perror("Setsockopt error");
    close(soquete);
    return -1;
  }

  return soquete;
}