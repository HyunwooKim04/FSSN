#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 65456
#define BUFFER_SIZE 1024

int main() {
    int serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;             //IP주소와 포트번호를 저장해주는 구조체
    socklen_t clientAddrSize = sizeof(clientAddr);  //client의 socket 주소는 변하기 때문에 따로 지정
    char buffer[BUFFER_SIZE];                      

    std::cout << "> echo server is activated" << std::endl;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error: socket creation falied" << std::endl;
        return 1;
    }
    
    memset(&serverAddr, 0, sizeof(serverAddr)); //serverAddr 구조체의 메모리 쓰레기값 정리 후 configure하는 4줄
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;    //자신의 32bit IP 할당, 랜카드가 하나일 때 INADDR_ANY
    serverAddr.sin_port = htons(PORT);          //포트번호를 16bit 빅엔디안 형식으로 설정

    if(bind(serverSocket, (sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: bind falied" << std::endl;
        close(serverSocket);
        return 1;
    }

    if(listen(serverSocket, 5) == -1) {
        std::cerr << "Error: listen failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    clientSocket = accept(serverSocket, (sockaddr*) &clientAddr, &clientAddrSize);
    if(clientSocket == -1) {
        std::cerr << "Error: accept failed" << std::endl;
        close(serverSocket);
        return 1;
    }
    std::cout << "> client connected" << std::endl;

    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if(bytesReceived <= 0) break;

        std::cout << "> echoed: " << buffer << std::endl;
        send(clientSocket, buffer, bytesReceived, 0);

        if(strcmp(buffer, "quit") == 0) break;
    }

    close(clientSocket);
    close(serverSocket);
    std::cout << "> echo server is de-activated" << std::endl;

    return 0;
}
