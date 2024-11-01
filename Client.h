#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

class SocketClient {
public:
    SocketClient();
    ~SocketClient();

    void start();
    void stop();
    std::string getUser1() const;  

private:
    void sendPeriodicMessages();
    void receiveMessages();

    SOCKET sockfd;
    std::thread senderThread;
    std::thread receiverThread;
    bool running;

    std::string user1;  
};

#endif // CLIENT_H
