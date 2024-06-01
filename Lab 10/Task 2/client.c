// Client side implementation of UDP client-server model 
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


void encrypt(char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] >= 'a' && data[i] <= 'z') {
            data[i] = (data[i] - 'a' + 3) % 26 + 'a';
        } else if (data[i] >= 'A' && data[i] <= 'Z') {
            data[i] = (data[i] - 'A' + 2) % 26 + 'A';
        } else if (data[i] >= '0' && data[i] <= '9') {
            data[i] = (data[i] - '0' + 1) % 10 + '0';
        }
    }
}

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr;

	// Opening File
	FILE *file = fopen("textFile.txt","r");
	if(file==NULL){
		perror("File not found ...");
	}

	// Store text file content in fileData array
	char fileData[MAXLINE];
	fread(fileData,sizeof(char),MAXLINE,file);
	fclose(file);

	encrypt(fileData);

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	connect (sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)); 
	int n, len; 
	
	send(sockfd, fileData, strlen(fileData),0);
	printf("Hello message sent.\n"); 
		
	n = recv(sockfd, (char *)buffer, MAXLINE, 0); 
	buffer[n] = '\0'; 
	printf("Server : %s\n", buffer); 
	
	
	close(sockfd); 
	return 0; 
} 

