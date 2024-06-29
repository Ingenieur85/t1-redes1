#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// OI

int create_raw_socket(char* nome_interface_rede) {
    // Cria arquivo para o socket sem qualquer protocolo
    int soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (soquete == -1) {
        fprintf(stderr, "Erro ao criar socket: Verifique se você é root!\n");
        exit(-1);
    }
 
    int ifindex = if_nametoindex(nome_interface_rede);
 
    struct sockaddr_ll endereco = {0};
    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(soquete, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
        fprintf(stderr, "Erro ao fazer bind no socket\n");
        exit(-1);
    }
 
    struct packet_mreq mr = {0};
    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        fprintf(stderr, "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente.\n");
        exit(-1);
    }
 
    return soquete;
}

#define BUF_SIZE 2048

void run_server(char* interface) {
    int sock = create_raw_socket(interface);
    char buffer[BUF_SIZE];

    printf("Server is waiting for a message...\n");

    // Receive message from client
    ssize_t num_bytes = recv(sock, buffer, BUF_SIZE, 0);
    if (num_bytes < 0) {
        perror("Server recvfrom failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Server received: %s\n", buffer);

    // Send acknowledgment to client
    char ack[] = "ACK";
    if (send(sock, ack, sizeof(ack), 0) < 0) {
        perror("Server sendto failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Server sent acknowledgment\n");
    close(sock);
}

void run_client(char* interface) {
    int sock = create_raw_socket(interface);
    char buffer[BUF_SIZE];

    char message[] = "Hello, Server!";
    if (send(sock, message, sizeof(message), 0) < 0) {
        perror("Client sendto failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Client sent: %s\n", message);

    // Receive acknowledgment from server
    ssize_t num_bytes = recv(sock, buffer, BUF_SIZE, 0);
    if (num_bytes < 0) {
        perror("Client recvfrom failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Client received acknowledgment: %s\n", buffer);
    close(sock);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s -s|-c <interface>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-s") == 0) {
        run_server(argv[2]);
    } else if (strcmp(argv[1], "-c") == 0) {
        run_client(argv[2]);
    } else {
        fprintf(stderr, "Invalid option: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
