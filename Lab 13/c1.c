#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int num1, num2;

    // Create the client socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server: %s:%d\n", SERVER_IP, SERVER_PORT);

    // Receive the welcome message from the server
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    printf("%s", buffer);

    while (1) {
        // Get input from the user
        printf("Enter two numbers (e.g., 10,5) or type EXIT to exit: ");
        fgets(buffer, BUFFER_SIZE - 1, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove the newline character

        // Check if the user wants to exit
        if (strcmp(buffer, "EXIT") == 0) {
            send(client_fd, buffer, strlen(buffer), 0);
            printf("Disconnecting from server...\n");
            break;
        }

        // Send the input to the server
        send(client_fd, buffer, strlen(buffer), 0);

        // Receive the result from the server
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        printf("%s", buffer);
    }

    close(client_fd);
    return 0;
}
