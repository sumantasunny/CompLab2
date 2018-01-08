#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 8080

char * readLineFromFile(int lineNo);
char * appendToTheFile(char * line);

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
		char message[256];
		int n;
		bzero(message,255);
		// buffer for storing client information 
		n = read(newsockfd, message, 255);
		message[n] = '\0';
		//reads information from socket to local buffer 
		printf("Client's message: %s\n", message);
		char reply[256];
		if(strcmp(message, "close") == 0)
		{
			//sprintf(reply, "Server: closing connection and exiting.....");
			printf("Server: closing connection and exiting.....\n");
			//n = write(newsockfd, reply, strlen(reply));
			break;
		}
		else
		{
			char * strMsg;
			char msg[10][1024];
			memset(msg, '\0', 10*1024);
			int i = 0, j = 0, k = 0, f = 0;
			for(i = 0; i < strlen(message); i++)
			{
				if(f == 0)
				{
					if(message[i] == ' ')
					{
						k = 0;
						j++;
						f = 1;
					}
					else
					{
						msg[j][k] = message[i];
						k++;
					}
				}
				else
				{
					msg[j][k] = message[i];
					k++;
				}
			}
			if(j == 1)
			{
				//printf("1\n");
				if(strcmp(msg[0], "READX") == 0)
				{
					int lineNo = atoi(msg[1]);
					strMsg = readLineFromFile(lineNo);
					sprintf(reply, "Server: read > %s", strMsg);
				}
				else if (strcmp(msg[0], "WRITEX") == 0)
				{
					//sprintf(reply, "Server: read > %s", "Write File");
					strMsg = appendToTheFile(msg[1]);
					sprintf(reply, "Server: write > %s", strMsg);
				}
				else
				{
					sprintf(reply, "Server: wrong operation code");
				}
			}
			else
			{
				sprintf(reply, "Server: wrong number of arguments");
			}
			n = write(newsockfd, reply, strlen(reply));
		}
		//writes message to the socket descriptor
		//free(message);
		//close(newsockfd); 
	} 
	close(newsockfd);
	close(sockfd);
}

char * readLineFromFile(int lineNo)
{
	FILE * fp;
	char line[1024];
	char * lineCopy;
	lineCopy = (char *)calloc(1024, 1);
	fp = fopen("data.txt", "r");
	while(fgets(line, sizeof(line), fp))
	{
		//puts(line);
		lineNo--;
		if(lineNo == 0)
		{
			if(line[strlen(line)-1] == '\n')
			{
				line[strlen(line)-1] = '\0';
			}
			strcpy(lineCopy, line);
			fclose(fp);
			return lineCopy;
		}
		else if(lineNo < 0)
		{
			break;
		}
	}
	strcpy(lineCopy, "Line doesn't exists");
	fclose(fp);
	return lineCopy;
}

char * appendToTheFile(char * line)
{
	FILE * fp;
	char * message;
	message = (char *)calloc(1024, 1);
	fp = fopen("data.txt", "a");
	fprintf(fp, "\n%s", line);
	fclose(fp);
	strcpy(message, "Success!");
	return message;
}