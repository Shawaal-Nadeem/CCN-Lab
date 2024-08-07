// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	// .sin_family-->The address family for the transport address
	servaddr.sin_family = AF_INET; // IPv4  This address family provides interprocess communication between processes that run on the same system or on different systems. Addresses for AF_INET sockets are IP addresses and port numbers. You can specify an IP address for an AF_INET socket either as an IP address 
	servaddr.sin_addr.s_addr = INADDR_ANY; // IP Address This is an IP address that is used when we don't want to bind a socket to any specific IP
	servaddr.sin_port = htons(PORT); 
	
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	 
	int len=sizeof(cliaddr); 
	int n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
	buffer[n] = '\0'; 
	write(1,buffer,strlen(buffer));
	sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &cliaddr,sizeof(cliaddr)); 
	printf("Hello message sent.\n"); 
	
	return 0; 
} 
