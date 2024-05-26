#include <iostream>

#include "tcp-adapter.hpp"

int main() {
    unsigned short port = 8554;
    TCP::Server server(5);
    int socket = server.OpenPort(port);
    if (socket < 0) {
        std::cout << "Cannot create socket\n";
        return 0;
    }
    std::cout << "Waiting for connections\n";
    auto clnt = server.WaitForConnection(socket);
    if (clnt == nullptr) {
        return 0;
    }
    std::cout << "Client has connected\n";
    while (clnt->IsConnected()) {
        std::vector<char> buf(10);
        clnt->Recv(buf, 10);
        std::cout << "\nReceived:\n" << buf.data() << "\n";
        clnt->Send(buf, buf.size());
    }
    server.CloseSocket(socket);
    std::cout << "Shut down.\n";
    return 0;
}
