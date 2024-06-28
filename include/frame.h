#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

#define INTERFACE "lo"

#define START_MARKER 0x7E
#define MAX_DATA_SIZE 63

/* Socket address structure*/
//struct sockaddr_ll socket_address;



// Define the custom frame structure
struct frame{
    uint8_t start;
    uint8_t size : 6;      /*Size of data section*/
    uint8_t sequence : 5;
    uint8_t type : 5;
    uint8_t data[MAX_DATA_SIZE];
    uint8_t crc;
};

#define ACK         0x00  
#define NACK        0x01 
#define LIST        0x0A 
#define DOWNLOAD    0x0B 
#define PLAY        0x10 
#define FD          0x11 
#define DATA        0x12 
#define END         0x1E
#define ERRO        0x1F 

// Define the error codes
#define ERRO_ACESSO_NEGADO  1
#define ERRO_NAO_ENCONTRADO 2
#define ERRO_DISCO_CHEIO    3


void construct_frame(struct frame *f, uint8_t size, uint8_t sequence, uint8_t type, const uint8_t *data);
int send_frame(int sockfd, struct frame *frame);
int receive_frame(int sockfd, struct frame *frame);
void data_from_frame(struct frame *frm, uint8_t *buffer);
//void request_file_list(int sockfd, struct sockaddr_ll *socket_address);
//void download_file(int sockfd, struct sockaddr_ll *socket_address);


#endif // FRAME_H