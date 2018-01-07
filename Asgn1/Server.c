#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
	printf("Starting server.....\n");
	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	//bzero initializes structure with zero 
	serv_addr.sin_family = AF_INET;
	//used for IPv4 
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	// if specified, socket is bind to any available local IP address 0.0.0.0   
	serv_addr.sin_port = htons(PORT);
	//specifies port number to bind with, htons converts short integer to network byte order 
	int sockfd;
	// used for storing socket descriptor, this only listens to any of the client’s connect request 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// AF_INET for IPv4, SOCK_STREAM is for creating TCP connection, 
	//SOCK_DGRAM is for creating UDP connection, 0 is specified TCP/UDP 
	//protocol, otherwise for RAW_STREAM valid IANA protocol needs to be 
	//specified. 
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
	{
		printf("ERROR on binding");
	} 
	//bind IP address and port number to create a socket

	listen(sockfd,5);  
	/* 
	first argument specifies socket descriptor where information from client will 
	be stored 
	Second argument defines the maximum length to which the queue of 
	pending connections for sockfd may grow. 
	*/ 

	struct sockaddr_in cli_addr;
	//storing client address
	socklen_t clilen;
	//storing length for client address, i.e. 32 bit integer 
	clilen = sizeof(cli_addr); 
	int newsockfd; //socket descriptor for client, this is exclusively returned for the specific client 
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //accept returns a socket descriptor through which client and server communicate 
	while(1)
	{
		char buffer[256];
		int n;
		bzero(buffer,255);
		// buffer for storing client information 
		n = read(newsockfd, buffer, 255);
		buffer[n] = '\0';
		//reads information from socket to local buffer 
		printf("Client's message: %s\n", buffer);
		char reply[256];
		sprintf(reply, "Server: I received > %s", buffer);
		n = write(newsockfd, reply, strlen(reply));
		//writes message to the socket descriptor
		//free(buffer);
		//close(newsockfd); 
	} 
	close(newsockfd);
	close(sockfd);
}
