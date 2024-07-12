// Server side implementation of UDP client-server model 
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


void decrypt(char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] >= 'a' && data[i] <= 'z') {
            data[i] = (data[i] - 'a' - 3 + 26) % 26 + 'a';
        } else if (data[i] >= 'A' && data[i] <= 'Z') {
            data[i] = (data[i] - 'A' - 2 + 26) % 26 + 'A';
        } else if (data[i] >= '0' && data[i] <= '9') {
            data[i] = (data[i] - '0' - 1 + 10) % 10 + '0';
        }
    }
}

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	listen (sockfd,5);
	int newfd;
	while(1)
	{
		int len= sizeof(cliaddr), n; 
		newfd= accept(sockfd, (struct sockaddr*) &cliaddr, &len);
		
		n = recv(newfd, (char *)buffer, MAXLINE,0); 
		buffer[n] = '\0'; 
		printf("Client : %s\n", buffer); 

		decrypt(buffer);

		send(newfd, buffer, strlen(buffer),0); 
		close(newfd);
	}
	return 0; 
} 

