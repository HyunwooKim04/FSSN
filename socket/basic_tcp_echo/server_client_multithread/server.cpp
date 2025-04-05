#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65456
#define BUFFER_SIZE 1024

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int receivedBytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (receivedBytes <= 0) break;

        std::cout << "> received: " << buffer << " (fd: " << clientSocket << ")\n";
        send(clientSocket, buffer, BUFFER_SIZE, 0);

        if (std::string(buffer) == "quit") break;
    }
    close(clientSocket);
    std::cout << "> client disconnected (fd: " << clientSocket << ")\n";
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 3);

    std::cout << "> server started on port " << PORT << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
        if (clientSocket < 0) continue;

        std::cout << "> client connected (fd: " << clientSocket << ")\n";

        std::thread serverThread(handleClient, clientSocket);
        serverThread.detach(); 
    }

    close(serverSocket);
    return 0;
}
