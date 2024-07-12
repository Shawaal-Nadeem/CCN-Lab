#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];
    fd_set read_fds;
    int max_fd, retval;

    // Create the server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the server socket to the address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding server socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("Error listening on server socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Clear the read_fds set
        FD_ZERO(&read_fds);

        // Add the server socket to the read_fds set
        FD_SET(server_fd, &read_fds);
        max_fd = server_fd;

        // Wait for a new connection or data from the client
        retval = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (retval < 0) {
            perror("Error in select()");
            close(server_fd);
            exit(EXIT_FAILURE);
        } else if (retval == 0) {
            // Timeout occurred (should never happen in this case)
            continue;
        }

        // Check if there is a new connection
        if (FD_ISSET(server_fd, &read_fds)) {
            client_len = sizeof(client_addr);
            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
            if (client_fd < 0) {
                perror("Error accepting client connection");
                continue;
            }

            printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Send the welcome message to the client
            sprintf(buffer, "Hi,\nPlease Enter any two numbers or type EXIT to exit\n");
            send(client_fd, buffer, strlen(buffer), 0);

            while (1) {
                // Wait for data from the client
                FD_ZERO(&read_fds);
                FD_SET(client_fd, &read_fds);
                retval = select(client_fd + 1, &read_fds, NULL, NULL, NULL);
                if (retval < 0) {
                    perror("Error in select()");
                    close(client_fd);
                    close(server_fd);
                    exit(EXIT_FAILURE);
                } else if (retval == 0) {
                    // Timeout occurred (should never happen in this case)
                    continue;
                }

                // Receive data from the client
                memset(buffer, 0, BUFFER_SIZE);
                retval = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
                if (retval < 0) {
                    perror("Error receiving data from client");
                    close(client_fd);
                    continue;
                } else if (retval == 0) {
                    // Client has disconnected
                    printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    close(client_fd);
                    break;
                }

                // Check if the client wants to exit
                if (strcmp(buffer, "EXIT") == 0) {
                    sprintf(buffer, "Goodbye!\n");
                    send(client_fd, buffer, strlen(buffer), 0);
                    close(client_fd);
                    printf("Client disconnected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    break;
                }

                // Process the client's input
                int num1, num2;
                if (sscanf(buffer, "%d,%d", &num1, &num2) == 2) {
                    sprintf(buffer, "Numbers: %d,%d\nSum: %d + %d = %d\nSub: %d - %d = %d\nDiv: %d / %d = %d\nMul: %d x %d = %d\n",
                            num1, num2, num1, num2, num1 + num2, num1, num2, num1 - num2, num1, num2, num1 / num2, num1, num2, num1 * num2);
                    send(client_fd, buffer, strlen(buffer), 0);
                } else {
                    sprintf(buffer, "Invalid input. Please enter two numbers or type EXIT to exit.\n");
                    send(client_fd, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
