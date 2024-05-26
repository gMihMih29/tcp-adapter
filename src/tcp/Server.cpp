#include "Server.hpp"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>

namespace TCP {

Server::Server(int queue_limit) : queue_limit_(queue_limit) {
    assert(
        queue_limit > 0 &&
        "Cannot use negative number for queue limit for incoming connections");
}

Server::~Server() {
    for (auto& i : sockets_) {
        CloseSocket(i.first);
    }
}

int Server::OpenPort(unsigned short port) {
    assert(port >= 0 && "Cannot open port with negative number");

    int sock;
    struct sockaddr_in serv_addr;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (bind(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) <
        0) {
        return -1;
    }

    if (listen(sock, queue_limit_) < 0) {
        return -1;
    }

    return sock;
}

int Server::CloseSocket(int socket) {
    assert(socket >= 0 && "Cannot open socket with negative number");
    auto it = sockets_.find(socket);
    assert(it != sockets_.end() && "Cannot close unopened socket");
    while (it != sockets_.end() && (*it).first == socket) {
        (*it).second->CloseConnection();
        ++it;
    }
    return sockets_.erase(socket) > 0 ? 0 : -1;
}

std::shared_ptr<Client> Server::WaitForConnection(int socket) {
    int client_socket;
    struct sockaddr_in client_addr;
    unsigned int client_len;

    client_len = sizeof(client_addr);
    client_socket =
        accept(socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
    if (client_socket < 0) {
        return std::shared_ptr<Client>(nullptr);
    }

    std::shared_ptr<Client> res(new Client(client_socket));
    sockets_.insert(std::make_pair(socket, res));
    return res;
}

}  // namespace TCP
