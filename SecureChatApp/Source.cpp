#include <iostream>
#include <winsock.h>
using namespace std;
#define Port 9909//The port number is a communication endpoint in a computer network, and it is used to uniquely identify a specific process to which data or messages should be sent. In the context of networking, a port is associated with an IP address to establish a connection between two devices.
struct sockaddr_in srv;//, the server address is represented by the struct sockaddr_in srv structure. This structure is used to configure the server's network address, including details like the address family, port number, and IP address.
fd_set fr, fe, fw;
int nMaxFd;
int nSocket;
int nArrClient[5];//for real life application u need more than 5
void ProcessNewMessages(int nClientSocket)
{
	cout << endl << "Processing the new message for client socket:" << nClientSocket; // Display a message indicating the start of message processing for a specific client socket
	char buff[256 + 1] = { 0, }; // Initialize a buffer to store the received message
	int nRet = recv(nClientSocket, buff, 256, 0);// Receive the message from the client socket
	if (nRet < 0) // Check if an error occurred during message reception
	{
		cout << endl << "Something wrong happeng..closing the connection for client";// Display an error message and close the connection for the client socket
		closesocket(nClientSocket);
		for (int nIndex = 0; nIndex < 5; nIndex++) // Remove the client socket from the array of client sockets (assuming nArrClient is a global array)
		{
			if (nArrClient[nIndex] == nClientSocket)
			{
				nArrClient[nIndex] = 0;
				break;
			}
		}
	}
	else
	{
		cout << endl << "the message recevied from client is:" << buff; // Display the received message from the client
		//send the response to client
		send(nClientSocket, "processed your request", 23, 0);
		cout << endl << "**********************************************"; // Display a separator for better visualization
	}
}
void ProccessTheNewRequest()
{//new connection
	if (FD_ISSET(nSocket, &fr))
	{
		int nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nSocket, NULL, &nLen);
		if (nClientSocket > 0)
		{
			//Put it into client fd_set
			int nIndex;
			for (nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nArrClient[nIndex] == 0)
				{
					nArrClient[nIndex] = nClientSocket;
					send(nClientSocket, "got the connection done successfully", 37, 0);
					break;
				}
			}
			if (nIndex == 5)
			{
				cout << endl << "No space for new connections";
			}

		}
	}
	else
	{
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (FD_ISSET(nArrClient[nIndex], &fr))
			{
				/*got the new message from the client
				just rec new message
				just queue that for new workes of your server to fulfill the request*/
				ProcessNewMessages(nArrClient[nIndex]);
			}
		}
	}
}
int main() {
	int nRet = 0;
	//uses wsa version 2.2
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0){
		cout << "WSAStartup failed" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);}
	else {
		cout << "WSAStartup succeeded" << endl;
	}
	//makes a socket that uses tcp
	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket == INVALID_SOCKET){
		cout << "Socket creation failed" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else{
		cout << "Socket created: " << nSocket << endl;
	}
	/* structure is initialized to set up the server's address information and specifies the address family, port (converted to network byte order using htons)allows connections from any available network interface */
	srv.sin_family = AF_INET;//sin_family: This field specifies the address family, and AF_INET indicates that the server will be using IPv4 addresses.
	srv.sin_port = htons(Port);//This means that the server will be reachable on port 9909 for incoming client connections.//You've used htons(port) to convert the port value to network byte order using htons (host to network short).
	srv.sin_addr.s_addr = INADDR_ANY;//This field specifies the IP address to which the server should bind. By setting it to INADDR_ANY, you are instructing the server to listen on all available network interfaces and IP addresses
	//if you want to enter a specified ip adress srv.sin_addr.s_addr = inet_addr ("127.0.0.01"); for the loopback address. //we collect This information bec, it is used when binding the server socket to a specific address and when accepting incoming client connections.
	memset(& (srv.sin_zero), 0, 8);
	//setsocketopt

	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);//if you want to change the adress every time then you chould use SO_EXCLUSIVEADDRUSE insted of SO_REUSEADDR
	if (!nRet)
	{
		cout << endl << "the set socket call success";
	}
	else
	{cout << endl << "the set socket call failed";
		closesocket(nSocket);
		WSACleanup();
		exit(EXIT_FAILURE);
	}
/*//blocking  vs non blocking sockets// optval =0 blocking optval != 0 non blockingu_long optval = 0;nRet = ioctlsocket(nsocket, FIONBIO, &optval)if (nRet != 0){cout << endl << "ioclsocket call failed";}else{			cout << endl << "ioclsocket call success";}*/
	//associates the socket with the server's address and port.
	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0) 
	{ cout << "Bind failed" << endl;
		closesocket(nSocket);
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Bind succeeded" << endl;
	}
	// starts listening on the socket for incoming client connections on the port the developer created
	nRet = listen(nSocket, 5);//5, represents the maximum number of pending connections.
	if (nRet < 0) 
	{   cout << "Listen failed" << endl;
		closesocket(nSocket);
		WSACleanup();
		return EXIT_FAILURE;
	}
	else
	{
		cout << "Listening on port " << Port << endl;
	}
	//keep listening
	nMaxFd = nSocket;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	while (1) {
		FD_ZERO(&fr);
		FD_ZERO(&fe);
		FD_ZERO(&fw);

		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);

		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nArrClient[nIndex] != 0)
			{
				FD_SET(nArrClient[nIndex], &fr);
				FD_SET(nArrClient[nIndex], &fe);
				
			}
		}

		nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);//The select function is used to wait for activity on the socket. 
		if (nRet > 0)
		{
			//when someone connects or communicates with a message over a dedicated  connection/*please remember that the socket listening to new clients and then communicating with the same client later are not the same.*//*after connection, you get one more socket to communicate with client you need to send/receive messages over the network using that new socket.*/
			cout << "someone connected";
			cout << "data on port ......processing now....";
			//process the request/*if (FD_ISSET(nsocket, &fe)){cout << endl << "there is an exception.just get away from here";}if (FD_ISSET(nsocket, &fw)){cout << endl << "ready to write sth";}if (FD_ISSET(nsocket, &fr)){cout << endl << "ready to read. sth new came up at the port";//acept the new connection}break;// Add code here for accepting incoming connections and handling them.
			ProccessTheNewRequest();
		}
		else if (nRet == 0)
		{
			// No incoming connections or data ready
		}
		else
		{
			//it fails 
			cout << "Select failed" << endl;
			closesocket(nSocket);
			WSACleanup();
			exit(EXIT_FAILURE);
		}
	}
	// Clean up the socket and Winsock when done
	closesocket(nSocket);
	WSACleanup();

	return 0;
}
