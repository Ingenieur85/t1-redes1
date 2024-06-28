#include "header.h"
#include "frame.h"

#if 0
void handle_request(int sockfd, struct sockaddr_ll *socket_address) {
    struct frame frame;
    receive_frame(sockfd, &frame);
    
    if (frame.type == 0x0A) { // List files request
        const char *file_list = "test.txt";
        construct_frame(&frame, 0x10, (uint8_t*)file_list, strlen(file_list));
        send_frame(sockfd, socket_address, &frame);
    } else if (frame.type == 0x0B) { // Download file request
        FILE *file = fopen("test.txt", "r");
        if (file) {
            char buffer[MAX_DATA_SIZE];
            size_t read_size = fread(buffer, 1, MAX_DATA_SIZE, file);
            construct_frame(&frame, 0x12, (uint8_t*)buffer, read_size);
            send_frame(sockfd, socket_address, &frame);
            fclose(file);
        }
    }
}
#endif

int main() {
    struct frame f;
    int sockfd = create_raw_socket("eth0"); // Replace "eth0" with your interface
    uint8_t buffer[MAX_DATA_SIZE];

    while (1) {
        /* Receive a frame */
        if (receive_frame(sockfd, &f) != -1) {
            data_from_frame(&f, buffer);
            printf("Frame received: %d\n", f.type);
            printf("Data: %s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}




