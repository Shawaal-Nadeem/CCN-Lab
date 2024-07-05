// Server side implementation of TCP concurrent server model
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

int main() {
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    pid_t pid;

    // Creating socket file descriptor
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Put the server socket into listening state
    if (listen(listenfd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Server loop to handle multiple clients
    while (1) {
        cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        if (connfd < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        if ((pid = fork()) == 0) {
            // Child process
            close(listenfd);
            char buffer[MAXLINE] = "Hello, this is the server";
            char modifiedBuffer[MAXLINE];
            int n;

            // Send character array to client
            send(connfd, buffer, strlen(buffer), 0);
            printf("Sent to client: %s\n", buffer);

            // Receive modified array from client
            n = recv(connfd, modifiedBuffer, MAXLINE, 0);
            modifiedBuffer[n] = '\0';
            printf("Received from client: %s\n", modifiedBuffer);

            close(connfd);
            exit(0);
        }

        close(connfd);
    }

    return 0;
}

