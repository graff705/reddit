#include "client.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <iomanip>
#include <sstream>

std::chrono::time_point<std::chrono::steady_clock> lastUser1Update;

SocketClient::SocketClient() : running(false), sockfd(INVALID_SOCKET) {
    setlocale(LC_ALL, "RU");
    WSADATA wsaData;

    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Failed to initialize Winsock.");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Error creating socket.");
    }

    //я хз зачем это нужно но без него работает плохо
    
  //  int timeout = 5000; // 5 секунд
  //  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
   // setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

   // sockaddr_in serverAddr = {};
   // serverAddr.sin_family = AF_INET;
  //  serverAddr.sin_port = htons(39970);

  //  if (inet_pton(AF_INET, "147.185.221.22", &serverAddr.sin_addr) <= 0) {
  //      closesocket(sockfd);
  //      WSACleanup();
   //     throw std::runtime_error("Invalid address or address not supported.");
//    }

  //  if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
   //     closesocket(sockfd);
   //     WSACleanup();
   //     throw std::runtime_error("Connection failed.");
 //   }

   // std::cout << "Successfully connected to the server." << std::endl; //сошушщц рцщшуа рщшцгуращц ру царшщу 
}

SocketClient::~SocketClient() {
    stop();
}

void SocketClient::start() {
    running = true;
    senderThread = std::thread(&SocketClient::sendPeriodicMessages, this);
    receiverThread = std::thread(&SocketClient::receiveMessages, this);
}

void SocketClient::stop() {
    if (running) {
        running = false;
        if (senderThread.joinable()) senderThread.join();
        if (receiverThread.joinable()) receiverThread.join();
        closesocket(sockfd);
        WSACleanup();
        std::cout << "Socket closed." << std::endl; 
    }
}

void SocketClient::sendPeriodicMessages() {
    std::string initialMessage = "SAK3URA\n";

    if (send(sockfd, initialMessage.c_str(), static_cast<int>(initialMessage.size()), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send initial message." << std::endl;
        running = false;
        return;
    }

    while (running) {
        std::string messageToSend;
        std::getline(std::cin, messageToSend);

        if (messageToSend.empty()) {
            std::cerr << "Сообщение не может быть пустым." << std::endl;
            continue;
        }

        messageToSend += "\n";
        if (send(sockfd, messageToSend.c_str(), static_cast<int>(messageToSend.size()), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message." << std::endl;
            running = false;
            return;
        }
    }
}

void SocketClient::receiveMessages() {
    char buffer[1024];
    while (running) {
        int bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::cerr << "Connection lost or error occurred." << std::endl;
            running = false;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::string message(buffer);
        std::cout << buffer << std::endl;

        // user1
        std::string user1Prefix = "user1:";
        size_t user1Pos = message.find(user1Prefix);
        if (user1Pos != std::string::npos) {
            user1 = message.substr(user1Pos + user1Prefix.length());
            user1.erase(0, user1.find_first_not_of(" \n\r\t"));
            user1.erase(user1.find_last_not_of(" \n\r\t") + 1);

            if (!user1.empty()) {
                lastUser1Update = std::chrono::steady_clock::now();
            }
        }

        // chat:
        std::string chatPrefix = "chat:";
        size_t chatPos = message.find(chatPrefix);
        if (chatPos != std::string::npos) {
            std::string chatMessage = message.substr(chatPos + chatPrefix.length());
            chatMessage.erase(0, chatMessage.find_first_not_of(" \n\r\t"));
            chatMessage.erase(chatMessage.find_last_not_of(" \n\r\t") + 1);

            if (!chatMessage.empty()) {
                std::cout << "Chat: " << chatMessage << std::endl;
            }
        }

        
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - lastUser1Update;
        if (elapsed.count() > 1.0) {
            user1.clear();  
        }
    }
}

std::string SocketClient::getUser1() const {
    return user1;
}
