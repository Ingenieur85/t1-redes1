#include "frame.h"
#include "header.h"

/* Function to populate a frame with its parameters */
void construct_frame(struct frame *frm, uint8_t size, uint8_t sequence, uint8_t type, const uint8_t *data) {
    frm->start = START_MARKER;
    frm->size = size;
    frm->sequence = sequence;
    frm->type = type;
    memset(frm->data, 0, MAX_DATA_SIZE);
    memcpy(frm->data, data, size);
    frm->crc = 0; // CRC check not implemented for now
}

/* Function to send a frame */
int send_frame(int sockfd, struct frame *frm) {
    int sent_bytes = send(sockfd, frm, sizeof(struct frame), 0);
    if (sent_bytes == -1) {
        perror("Send Frame");
        exit(-1);
    }
    return sent_bytes;
}

/* Function to receive a frame */
int receive_frame(int sockfd, struct frame *frm) {
    int recv_bytes = recv(sockfd, frm, sizeof(struct frame), 0);
    if (recv_bytes == -1) {
        perror("Receive Frame");
        exit(-1);
    }
    return recv_bytes;
}

/* Function to read data from a packet */
void data_from_frame(struct frame *frm, uint8_t *buffer) {
    memcpy(buffer, frm->data, frm->size);
}

/*
void request_file_list(int sockfd, struct sockaddr_ll *socket_address) {
    struct frame frame;
    construct_frame(&frame, 0x0A, NULL, 0);
    send_frame(sockfd, socket_address, &frame);
    receive_frame(sockfd, &frame);
    printf("Available files: %s\n", frame.data);
}

void download_file(int sockfd, struct sockaddr_ll *socket_address) {
    struct frame frame;
    construct_frame(&frame, 0x0B, (uint8_t*)"test.txt", strlen("test.txt"));
    send_frame(sockfd, socket_address, &frame);
    receive_frame(sockfd, &frame);
    printf("Received data: %s\n", frame.data);
}
*/