// Client side implementation of TCP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

void swapEvenOddIndices(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len - 1; i += 2) {
        char temp = str[i];
        str[i] = str[i + 1];
        str[i + 1] = temp;
    }
}

void communicate_with_server() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // Receive character array from server
    int n = recv(sockfd, buffer, MAXLINE, 0);
    buffer[n] = '\0';
    printf("Received from server: %s\n", buffer);

    // Modify the array by swapping even and odd indices
    swapEvenOddIndices(buffer);

    // Send modified array back to server
    send(sockfd, buffer, strlen(buffer), 0);
    printf("Sent back to server: %s\n", buffer);

    close(sockfd);
}

int main() {
    communicate_with_server();
    return 0;
}

