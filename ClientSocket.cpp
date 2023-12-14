#include <iostream>
#include <winsock.h>
#include <thread>
#include <chrono>
#include <mutex>

#define PORT 9909
using namespace std;
int nClientSocket;
struct sockaddr_in srv;
char buff[255] = { 0, };

std::mutex mtx;

void loop1() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        recv(nClientSocket, buff, 256, 0);
        std::lock_guard<std::mutex> lock(mtx);
        cout << endl << buff << endl;
    }
}

void loop2() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        fgets(buff, 256, stdin);
        send(nClientSocket, buff, 256, 0);
    }
}

int main()
{
    int nRet = 0;
    WSAData ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        cout << endl << "WSAStartup failed";
        WSACleanup();
        return (EXIT_FAILURE);
    }

    nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (nClientSocket < 0)
    {
        cout << endl << "socket call failed";
        WSACleanup();
        return (EXIT_FAILURE);
    }
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&srv.sin_zero, 0, 8);
    nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (nRet < 0)
    {
        cout << endl << "connect failed";
        WSACleanup();
        return (EXIT_FAILURE);
    }
    else
    {
        cout << endl << "Connected to the server ";
        char buff[255] = { 0, };
        char userMessage[255] = { 0, };
        // Concatenate the predefined message and user input
        recv(nClientSocket, buff, 256, 0);
        std::cout << endl << "Just press enter to see the message received from the server";
        cin.ignore(); // Ignore any newline character left in the buffer
        cout << endl << buff;
        cout << endl << "Now send your message to server:";
        // Create two threads
        thread thread1(loop1);
        thread thread2(loop2);
        // Wait for both threads to finish (which will never happen in this case)
        thread1.join();
        thread2.join();
        
    }
}
