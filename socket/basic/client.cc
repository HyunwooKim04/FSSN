// client.cc

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65456
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    sockaddr_in serverAddr{};
    char buffer[BUFFER_SIZE];

    std::cout << "> echo-client is activated" << std::endl;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1) {
        perror("socket() failed");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if(connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("connect() failed");
        close(clientSocket);
        return 1;
    }

    while(true) {
        std::cout << "> ";
        std::string sendMsg;
        std::getline(std::cin, sendMsg);

        send(clientSocket, sendMsg.c_str(), sendMsg.length(), 0);

        memset(buffer, 0, BUFFER_SIZE);
        int byteReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if(byteReceived <= 0) break;
        std::cout << "> received: " << buffer << std::endl;
        if(sendMsg == "quit") break;
    }

    close(clientSocket);
    std::cout << "> echo-client is de-activated" << std::endl;

    return 0;
}
