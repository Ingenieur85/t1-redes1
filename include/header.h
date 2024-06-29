#ifndef HEADER_H
#define HEADER_H

/* todtsocket includes*/
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

int create_raw_socket(char* nome_interface_rede);
int ConexaoRawSocket(char *device);

#endif // HEADER_H