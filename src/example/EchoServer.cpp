#include <iostream>

#include "tcp-adapter.hpp"

int main() {
    unsigned short port = 8554;
    TCP::Server server;
    if (!server.OpenPort(port, 5)) {
        std::cout << "Cannot open port\n";
        return 0;
    }
    std::cout << "Waiting for connections\n";
    auto clnt = server.WaitForConnection(port);
    if (clnt == nullptr) {
        return 0;
    }
    std::cout << "Client has connected\n";
    while (clnt->IsConnected()) {
        std::vector<char> buf(10);
        int recv_size = clnt->Recv(buf.data(), buf.size());
        if (recv_size == 0) {
            break;
        }
        std::cout << "\nReceived:\n" << buf.data() << "\n";
        clnt->Send(buf.data(), recv_size);
    }
    server.ClosePort(port);
    std::cout << "Shut down.\n";
    return 0;
}
