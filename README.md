# Socket-Programming

A Base64 Encoding Communication Protocol between server and client to communicate with each other based on TCP Sockets. Implemented a concurrent server to accept connections from multiple clients and serve all of them concurrently. 

The client and server interact using three types of messages: 
- Type 1(Encoded text message)
- Type 2(ACK message)
- Type 3(Connection Termination message)

Properties
- Both client and server work on TCP sockets.
- Server should run before client as client will connect to the server as soon as it runs.
- Server waits for TCP connection from client. 
- Server starts a new child thread ( using fork() ) for every client so that multiple clients can be handled concurrently.

WorkFlow
- Once the client-server connection is setup, the client asks user to give input data which is then encoded using Base64 encoding (max input lenght allowed is 1000).
- The client sends the encoded message to the server(Type 1 msg).
- After receiving the msg, server decodes the message and prints both encoded and decoded message and sends an "ACK" to the client(Type 2 msg).
- Note: The "ACK" sent by the server is also encoded, and is decoded by the client.
- Server and client remain in the loop to send any number of messages which the client wishes.
- When user doesn't wish to send more messages, client sends a type 3 msg to server and connections is closed.

To run the code inside src folder:

Compiling:
gcc server.c -o server
gcc client.c -o client

Running:

Starting the server:
Execute the following command taking the server port number as argument
./server <port number> 
For example:  ./server 4000

Starting the client:
Execute the following command taking the server ip address and server port number as arguments.The server is bind to all available local interface ips. 
./client <server ip address> <server port number>
