#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct hostent *server;
	server = gethostbyname("localhost");
	//this command resolves host 
	//address to corresponding IP address 
	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr)); 
	
	//initializes buffer
	serv_addr.sin_family = AF_INET; 
	
	//for IPv4 family
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); /*copy server IP address*/
	serv_addr.sin_port = htons(PORT); //defining port number
	//close(sockfd);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
		printf("ERROR connecting\n");
		exit(0);
	}
	// initiating connect request to the server  
	char * buffer;
	int n; 
	size_t m;
	
	while(1)
	{
		buffer = (char *)malloc(256);
		//bzero(buffer,255);
		getline(&buffer, &m, stdin);
		buffer[strlen(buffer)-1] = '\0';
		n = write(sockfd, buffer, strlen(buffer));
		//bzero(buffer,255);
		n = read(sockfd,buffer,255);
		buffer[n] = '\0';
		printf("%s\n", buffer);
	}
	close(sockfd);
	return 0;
}
