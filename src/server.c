#include "base_64_decoder.c"
#include "base_64_encoder.c"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

void recvAndAck(int c_fd,struct sockaddr_in *client)
{
	char *ip = inet_ntoa(client->sin_addr); //ip address of client
	int port = client->sin_port; // port of client
	printf("\nNEW CLEINT CONNECTION (%s : %d) ESTABLISHED\n",ip,port);

	char buffer[1500]; //buffer to store sending value

	//wait for msg from client till close connection request is recieved
	while(1)
	{
		bzero(buffer,1500); 
		int st = read(c_fd,buffer,1500); //read msg from socket
		
		//first char of msg is msg type. 3 refers to close connection
		if(buffer[0]=='3')
			break;

		if(buffer[0]=='1')
		{
			printf("\nMessage received from client %s : %d\n\tEncoded Message: %s\n",ip,port,buffer+1); //print encoded msg
			printf("\tDecoded Message: %s\n",decode(buffer+1)); //print decoded msg

			bzero(buffer,1500);
			buffer[0]='2'; //write to buffer. 2 refers to acknowledgement
			strcpy(buffer+1,encode("ACK")); //append encoded value of "ACK" after msg type
			write(c_fd,buffer,strlen(buffer)); //send acknowledgement
		}
		else
			break;
	}

	close(c_fd); //close connection
	printf("\nCLEINT CONNECTION (%s : %d) CLOSED\n",ip,port);
	exit(0);
}
int main(int argc, char *argv[])
{
	struct sockaddr_in server,client;
	int serverSocket = socket(AF_INET,SOCK_STREAM,0); //AF_NET is for IPv4, SOCK_STREAM indicates that TCP socket is created
	
	//couldnt create socket
	if(serverSocket == -1)
	{ 
		printf("\nSOCKET CREATION FAILURE\n");
		exit(0);
	}
	server.sin_family = AF_INET;  //code for address family - IPv4
	server.sin_addr.s_addr = INADDR_ANY; //address of host, binds the socket to all available local interfaces
	server.sin_port = htons(atoi(argv[1]));  //arguement 1 is server port number entered by user
	memset(&server.sin_zero,8, 0); 
	socklen_t length = sizeof(struct sockaddr_in);
	
	//if port is already in use binding fails
	if(bind(serverSocket,(struct sockaddr*) &server, length) < 0)
	{ 
		printf("\nBINDING FAILED\n");
		exit(0);
	}

	//listen to the current socket
	if(listen(serverSocket,20) == -1)
	{
		printf("\nLISTEN FAILED\n");
		exit(0);
	}
	printf("SERVER WORKING\n");
	fflush(stdout);
	while(1)
	{
		int clientSocket = accept(serverSocket,(struct sockaddr *) &client,&length); //client tries to connect
		
		// client connection was not made
		if(clientSocket < 0)
		{ 
			printf("\nSERVER-CLIENT CONNECTION COULD NOT BE ESTABLISHED\n");
			exit(0);
		}
		int status = fork(); //fork to create a child process to handle this client, so that multiple clients can be handled concurrently
		switch(status)
		{
			// error while creating child process
	 		case -1:
				printf("\nCOULDN'T ESTABLISH CONNECTION\n");
				break;
			//child process
			case 0:
				close(serverSocket); //server socket is handled by parent process
				recvAndAck(clientSocket,&client); // this functions handles a client
				break;
			//parent process 
			default:
				close(clientSocket);  // client socket is now handled by child process	
		}
		
	}
	return 0;


}
