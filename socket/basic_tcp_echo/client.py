import socket

HOST = '127.0.0.1'
PORT = 65456

print('> echo-client is activated')

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as clientSocket:
    clientSocket.connect((HOST, PORT))
    while True:
        sendMsg = input("> ")
        clientSocket.sendall(bytes(sendMsg, 'utf-8'))
        recvData = clientSocket.recv(1024)
        print('> received:', recvData.decode('utf-8'))
        if sendMsg == "quit":
            break

print('> echo-client is de-activated')
