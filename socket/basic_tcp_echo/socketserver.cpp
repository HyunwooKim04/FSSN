#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65456
#define BUFFER_SIZE 1024

void MyTCPSocketHandler(int clientSocket, sockaddr_in clientAddr) {
    char buffer[BUFFER_SIZE];

    std::cout << "> client connected by IP address "
              << inet_ntoa(clientAddr.sin_addr)
              << " with Port number "
              << ntohs(clientAddr.sin_port) 
              << std::endl;

    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if(bytesReceived <= 0) break;

        std::cout << "> echoed: " << buffer << std::endl;
        send(clientSocket, buffer, BUFFER_SIZE, 0);
        if(strcmp(buffer, "quit") == 0) break;
    }

    close(clientSocket);
    std::cout << "> client is de-activated" << std::endl;
}

int main() {
    int serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1) {
        std::cerr << "Error: Socket creation failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if(bind(serverSocket, (sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: bind failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    if(listen(serverSocket, 5) == -1) {
        std::cerr << "Error: listen failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "> echo-server is activated" << std::endl;

    clientSocket = accept(serverSocket, (sockaddr*) &clientAddr, &clientAddrSize);
    if(clientSocket == -1) {
        std::cerr << "Error: accpet failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    MyTCPSocketHandler(clientSocket, clientAddr);

    close(serverSocket);

    std::cout << "> Server is de-activated" << std::endl;
    return 0;
}
