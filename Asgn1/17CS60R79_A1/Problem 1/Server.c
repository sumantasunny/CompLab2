/*############################################################################################################
# Author:					Sumanta Dey (17CS60R79)
# Assignment:				Assignment 1
# Version:					0.2
# Created Date:				03-Jan-2017
# Last Modified Date:		09-Jan-2017
# Last Modified By:			Sumanta Dey
# Reason of Modification:	Documentation
###################################### :: Description :: ###############################################################
# Objective: To creates the server
############################################################################################################*/
#include <stdio.h>				/* printf()					*/
#include <sys/socket.h>			/* socket()					*/
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>				/* puts(), sprintf()		*/
#include <netdb.h>
#include <unistd.h>

#define PORT 8080

void createServer();

//The main function
int main(int argc, char const *argv[])
{
	createServer();
	return 0;
}

//Creates the server and handles the client requests
void createServer()
{
	int sockfd;  //used for storing socket descriptor, this only listens to any of the client’s connect request
	int n;
	char message[256], reply[256];
	printf("Starting server.....\n");
	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr)); //bzero initializes structure with zero 
	serv_addr.sin_family = AF_INET; //used for IPv4 
	serv_addr.sin_addr.s_addr = INADDR_ANY; //if specified, socket is bind to any available local IP address 0.0.0.0   
	serv_addr.sin_port = htons(PORT); //specifies port number to bind with, htons converts short integer to network byte order 
	
	/*AF_INET for IPv4, SOCK_STREAM is for creating TCP connection, 
	  SOCK_DGRAM is for creating UDP connection, 0 is specified TCP/UDP 
	  protocol, otherwise for RAW_STREAM valid IANA protocol needs to be specified.*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//bind IP address and port number to create a socket
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
	{
		printf("ERROR on binding");
	}

	/*first argument specifies socket descriptor where information from
	  client will be stored. Second argument defines the maximum length
	  to which the queue of pending connections for sockfd may grow.*/
	listen(sockfd,5);

	struct sockaddr_in cli_addr; //storing client address
	socklen_t client; //storing length for client address, i.e. 32 bit integer 
	int newsockfd; //socket descriptor for client, this is exclusively returned for the specific client
	client = sizeof(cli_addr); 
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client); //accept returns a socket descriptor through which client and server communicate
	while(1)
	{
		bzero(message,255); 		//buffer for storing client message
		memset(reply, '\0', 256);	//buffer for storing server reply
		n = read(newsockfd, message, 255); //reads information from socket to local buffer
		message[n] = '\0'; 
		printf("Client : %s\n", message);
		if(strcmp(message, "close") == 0)
		{
			printf("Server : closing connection and exiting.....\n");
			break;
		}
		else
		{
			sprintf(reply, "%s", "got your message");
			n = write(newsockfd, reply, strlen(reply));
			printf("Server : %s\n", reply);
		}
	} 
	close(newsockfd); //Close the Client specific socket
	close(sockfd);  //Close the Server socket
}