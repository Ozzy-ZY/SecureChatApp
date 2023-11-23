#include <iostream>
#include <winsock2.h>
using namespace std;
#define port 9909

struct sockaddr_in srv;
fd_set fr, fe, fw;
int nmaxfd;
int nsocket;

void ProccessTheNewRequest()
{//new connection
	if (FD_ISSET(nsocket, &fr))
	{
		int nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nsocket, NULL, &nLen)
	}
}

int main() {
	int nRet = 0;
	//uses wsa version 2.2
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		cout << "WSAStartup failed" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "WSAStartup succeeded" << endl;
	}
	//makes a socket that uses tcp
	nsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nsocket == INVALID_SOCKET) {
		cout << "Socket creation failed" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);;
	}
	else {
		cout << "Socket created: " << nsocket << endl;
	}
	/* structure is initialized to set up the server's address information and specifies
	the address family, port (converted to network byte order using htons)allows connections
	from any available network interface */
	srv.sin_family = AF_INET;//AF_INET indicates that this server will be using IPv4 addresses.
	srv.sin_port = htons(port);//You've used htons(port) to convert the port value to network byte order
	srv.sin_addr.s_addr = INADDR_ANY;//This field specifies the IP address to which the server should bind. By setting it to INADDR_ANY, you are instructing the server to listen on all available network interfaces and IP addresses
	//if you want to enter a specified ip adress srv.sin_addr.s_addr = inet_addr ("127.0.0.01");

	//setsocketopt
	int noptval = 0;
	int noptlen = sizeof(noptval);
	nRet = setsockopt(nsocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&noptval, noptlen);//if you want to change the adress every time then you chould use SO_EXCLUSIVEADDRUSE insted of SO_REUSEADDR
	if (!nRet)
	{
		cout << endl << "the set socket call success";
	}
	else
	{
		cout << endl << "the set socket call failed";
		closesocket(nsocket);
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	/*//blocking  vs non blocking sockets
	// optval =0 blocking optval != 0 non blocking
	u_long optval = 0;
	nRet = ioctlsocket(nsocket, FIONBIO, &optval)
		if (nRet != 0)
		{
			cout << endl << "ioclsocket call failed";
		}
		else
		{
			cout << endl << "ioclsocket call success";
		}*/

		//associates the socket with the server's address and port.
	nRet = bind(nsocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		cerr << "Bind failed" << endl;
		closesocket(nsocket);
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Bind succeeded" << endl;
	}
	// starts listening on the socket for incoming client connections on the port the developer created
	nRet = listen(nsocket, 5);//5, represents the maximum number of pending connections.
	if (nRet == SOCKET_ERROR) {
		cerr << "Listen failed" << endl;
		closesocket(nsocket);
		WSACleanup();
		return EXIT_FAILURE;
	}
	else {
		cout << "Listening on port " << port << endl;
	}
	//keep listening
	int nmaxfd = nsocket;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	while (1) {
		FD_ZERO(&fr);
		FD_ZERO(&fe);
		FD_ZERO(&fw);
		FD_SET(nsocket, &fr);
		FD_SET(nsocket, &fe);
		cout << endl << "before select call:" << fr.fd_count;
		nRet = select(nmaxfd + 1, &fr, &fw, &fe, &tv);//The select function is used to wait for activity on the socket. 
		if (nRet > 0)
		{
			//when someone connects or communicates with a message over a dedicated  connection
			cout << "someone connected";
			cout << "data on port ......processing now....";
			//process the request
			if (FD_ISSET(nsocket, &fe))
			{
				cout << endl << "there is an exception.just get away from here";
			}
			if (FD_ISSET(nsocket, &fw))
			{
				cout << endl << "ready to write sth";
			}
			if (FD_ISSET(nsocket, &fr))
			{
				cout << endl << "ready to read. sth new came up at the port";

			}
			break;
			// Add code here for accepting incoming connections and handling them.
		}
		else if (nRet == 0)
		{
			// No incoming connections or data ready
			cout << endl << "nothing on port:" << port << fr.fd_count;
		}
		else
		{
			//it fails 
			cout << "Select failed" << endl;
			closesocket(nsocket);
			WSACleanup();
			exit(EXIT_FAILURE);
		}
		cout << endl << "after select call:" << fr.fd_count;



	}
	// Clean up the socket and Winsock when done
	closesocket(nsocket);
	WSACleanup();

	return 0;
}