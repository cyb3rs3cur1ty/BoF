#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUF_SIZE 256

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(struct sockaddr_in);
    char buf[BUF_SIZE];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, addr_len) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len)) == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    // Receive data from client
    ssize_t bytes_received;
    if ((bytes_received = recv(client_fd, buf, BUF_SIZE, 0)) == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    printf("Received data from client: %s\n", buf);

    // Vulnerable code - buffer overflow vulnerability
    char vuln_buf[BUF_SIZE];
    strcpy(vuln_buf, buf);

    // Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
