#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65456
#define BUFFER_SIZE 1024

void recvHandler(int clientSocket) {
    char buffer[BUFFER_SIZE];

    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        int byteRecieved = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if(byteRecieved <= 0) break;
        std::cout << "> recieved: " << buffer << std::endl;
        if(strcmp(buffer, "quit") == 0) break;
    }
}

int main() {
    std::cout << "> client is activated" << std::endl;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1) {
        std::cerr << "Error: Socket creadtion failed" << std::endl;
        close(clientSocket);
        return 1;
    }

    sockaddr_in sockAddr;
    socklen_t sockAddrSize = sizeof(sockAddr);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_port = ntohs(PORT);

    if(connect(clientSocket, (sockaddr*)& sockAddr, sockAddrSize) == -1) {
        std::cerr << "Error: connection failed" << std::endl;
        close(clientSocket);
        return 1;
    }

    std::thread clientThread(recvHandler, clientSocket);
    clientThread.detach();

    while(true) {
        std::string sendMsg;
        std::getline(std::cin, sendMsg);

        send(clientSocket, sendMsg.c_str(), sendMsg.length(), 0);
        if(sendMsg == "quit") break;
    }

    close(clientSocket);
    std::cout << "client is de-activated" << std::endl;
    return 0;
}
