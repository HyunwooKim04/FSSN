import socketserver

class MyTCPSocketHandler(socketserver.BaseRequestHandler):
   
    def handle(self):
        print('> client connected by IP address {0} with Port number {1}'.format(self.client_address[0], self.client_address[1])) # client_address는 튜플
        while True:
            # [=start=]
            RecvData = self.request.recv(1024)
            print('> echoed:', RecvData.decode('utf-8'))
            self.request.sendall(RecvData)
            if RecvData.decode('utf-8') == 'quit':
                break
            # [==end==]

if __name__ == "__main__":
    HOST, PORT = '127.0.0.1', 65456
    print('> echo-server is activated')
    # Create the server, binding to localhost on port 9999
    with socketserver.TCPServer((HOST, PORT), MyTCPSocketHandler) as server:
        server.serve_forever()
    print('> echo-server is de-activated')
