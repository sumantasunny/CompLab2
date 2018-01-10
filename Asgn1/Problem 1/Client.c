/*############################################################################################################
# Author:					Sumanta Dey (17CS60R79)
# Assignment:				Assignment 1
# Version:					0.2
# Created Date:				03-Jan-2017
# Last Modified Date:		09-Jan-2017
# Last Modified By:			Sumanta Dey
# Reason of Modification:	Documentation
###################################### :: Description :: ###############################################################
# Objective: To creates the client
############################################################################################################*/
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#define PORT 8080

void startClient();

//The main function
int main(int argc, char const *argv[])
{
	startClient();
	return 0;
}

//Creates the client and send the request to the server
void startClient()
{
	char buffer[256];
	int n; 
	size_t m;
	int sockfd;
	struct hostent *server;
	struct sockaddr_in serv_addr;
	printf("Starting client.....\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0); //Creating the SOCKET(domain, type, protocol)
	//START :: To collect the server details
	server = gethostbyname("localhost"); //resolves the localhost
	bzero((char *) &serv_addr, sizeof(serv_addr)); //initialize the server address
	serv_addr.sin_family = AF_INET; //for IPv4 family
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //copy(byte) the server(localhost) IP address
	serv_addr.sin_port = htons(PORT); //defining port number
	//END :: To collect the server details
	int status = connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)); //initiating connect request to the server
	if (status < 0)
	{
		printf("ERROR connecting\n");
		exit(1);
	}
	while(1)
	{
		memset(buffer, '\0', sizeof(buffer)); //initialize the buffer
		printf("Client : ");
		fgets(buffer, sizeof(buffer), stdin); //Take the user input
		if(buffer[strlen(buffer)-1] == '\n')
		{
			buffer[strlen(buffer)-1] = '\0'; //removes the new line character
		}
		n = write(sockfd, buffer, strlen(buffer)); //write the user input to the socket
		if(strcmp(buffer, "close") == 0)
		{
			break;
		}
		else
		{
			memset(buffer, '\0', sizeof(buffer)); //reset the buffer
			n = read(sockfd,buffer,255); //read from the socket(server message)
			buffer[n] = '\0';
			printf("Server : %s\n", buffer);
		}
	}
	close(sockfd); //Close the SOCKET
}