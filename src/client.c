#include "base_64_decoder.c"
#include "base_64_encoder.c"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 


void getInput(char buffer[])
{
	char inp;
	printf("Write message to send\n");
	bzero(buffer, 1500); 
	int i = 0;
	
	//maximum input size is 1000
	while(1)
	{
		scanf("%c", &inp); //read message character by character    
		if(inp == '\n') 
			break;
		if (i == 1000)
		{
			printf("Only first 1000 characters of message are being sent\n");
			break;
		}
		buffer[i++] = inp; 
	}
}

void closeConnection(char buffer[], int clientSocket)
{
	buffer[0] =  '3'; 	 //set msg type
	strcpy(buffer+1, encode("connection_close")); // append encoded value of "connection_close" after msg type
	printf("CONNCECTION CLOSED\n"); 
	write(clientSocket, buffer, strlen(buffer)); // write in socket
}

int main(int argc, char *argv[])
{
	struct sockaddr_in server; 
	char inp; //to read user input character by character 
	int clientSocket = socket(AF_INET,SOCK_STREAM,0); //AF_NET is for IPv4, SOCK_STREAM indicates that TCP socket is created
	char buffer[1500]; //buffer to store recieving and modified sending value
	
	//socket couldn't be created
	if(clientSocket == -1)
	{
		printf("SOCKET CREATION FAILURE\n");
		return 0;
	} 

	server.sin_family = AF_INET; //address family - ipv4

	//server ip address, inputted by user
	if(inet_aton(argv[1],&server.sin_addr)==0)
	{ 
		printf("SERVER IP ADDRESS ERROR\n");
		return 0;
	} 

	int serverPort = atoi(argv[2]); // server port, inputted by user
	server.sin_port = htons(serverPort); 
	socklen_t length = sizeof(struct sockaddr_in); 

	// connection establishment failure
	if(connect(clientSocket,(struct sockaddr *) &server, length) == -1)
	{ 
		printf("COULDN'T CONNECT TO THE SERVER\n");
		exit(0);
	} 

	while(1)
	{
		
		printf("Send message?\nPress 'y' for YES or any other key for NO and press enter: \n");
		scanf("%c", &inp);

		//n impies send close connection request to server
		if(inp != 'y')
		{
			closeConnection(buffer,clientSocket);
			break; 
		}

		getchar();
		getInput(buffer);

		strcpy(buffer+1, encode(buffer)); // append encoded value of msg after msg type
		buffer[0] =  '1'; //set msg type as 1
		write(clientSocket, buffer, strlen(buffer)); //write to socket
		bzero(buffer, 1500);
		int received = read(clientSocket, buffer, 50); //recieve acknowledgement from server
		strcpy(buffer+1, decode(buffer+1)); //decode msg

		//msg type 2 indicates acknowledgement
		if(buffer[0]!='2')
		{
			printf("Acknowledgement could not received.\nResend the message.\n"); 
			continue; 
		}

		
		printf("Message received from server having IP %s and port %d\n",argv[1], serverPort); 
		printf("%s\n", buffer+1);
	}

	close(clientSocket); // close the socket

return 0;
}
