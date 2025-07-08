// server.cc

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65456
#define BUFFER_SIZE 1024

int main() {
    int serverSocket, clientSocket;
    sockaddr_in serverAddr{};
    sockaddr_in clientAddr{};
    socklen_t addrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    std::cout << "> echo-server is activated" << std::endl;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1) {
        perror("socket() failed");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if(bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind() failed");
        close(serverSocket);
        return 1;
    }

    if(listen(serverSocket, 1) == -1) {
        perror("listen() failed");
        close(serverSocket);
    }

    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrSize);
    if(clientSocket == -1) {
        perror("accept() failed");
        close(serverSocket);
        return 1;
    }

    std::cout << "> Client connected by IP address "
    << inet_ntoa(clientAddr.sin_addr)
    << " with Port number "
    << ntohs(clientAddr.sin_port)
    << std::endl;

    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if(bytesRead <= 0) break;
        std::cout << "> echoed: " << buffer << std::endl;
        send(clientSocket, buffer, bytesRead, 0);
        if(std::string(buffer).substr(0, 4) == "quit") break;
    }

    close(clientSocket);
    close(serverSocket);
    std::cout << "> echo-server is de-activated" << std::endl;
}
