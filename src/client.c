#include "header.h"
#include "frame.h"

int main() {
    struct frame f;
    int sockfd = create_raw_socket("eth0");
  
    uint8_t buffer[MAX_DATA_SIZE] = {1};
    buffer[0] = 0;
    construct_frame(&f, 1, 0, DATA, buffer);

    send_frame(sockfd, &f);


    //request_file_list(sockfd, &socket_address);
    //download_file(sockfd, &socket_address);

    close(sockfd);
    return 0;
}

