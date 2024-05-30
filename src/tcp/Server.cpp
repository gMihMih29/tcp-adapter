#include "Server.hpp"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>

namespace TCP {

Server::~Server() {
    for (auto& i : sockets_by_port_) {
        ClosePort(i.first);
    }
}

bool Server::OpenPort(unsigned short port, int queue_limit) {
    assert(sockets_by_port_.find(port) == sockets_by_port_.end() &&
           "Cannot open one port twice");
    assert(
        queue_limit > 0 &&
        "Cannot use negative number for queue limit for incoming connections");
    int sock;
    struct sockaddr_in serv_addr;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return false;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (bind(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) <
        0) {
        return false;
    }

    if (listen(sock, queue_limit) < 0) {
        return false;
    }
    sockets_by_port_[port] = sock;
    return true;
}

bool Server::ClosePort(unsigned short port) {
    auto port_sock = sockets_by_port_.find(port);
    assert(port_sock != sockets_by_port_.end() && "Cannot close unopened port");

    auto sock_client = client_managers_by_socket_.find(port_sock->second);
    while (sock_client != client_managers_by_socket_.end() &&
           sock_client->first == port_sock->second) {
        sock_client->second->CloseConnection();
        ++sock_client;
    }
    client_managers_by_socket_.erase(sock_client->first);
    return close(port_sock->second) >= 0;
}

std::shared_ptr<ClientManager> Server::WaitForConnection(unsigned short port) {
    auto port_sock = sockets_by_port_.find(port);
    assert(port_sock != sockets_by_port_.end() && "Cannot use unopened port");

    int client_socket;
    struct sockaddr_in client_addr;
    unsigned int client_len;
    client_len = sizeof(client_addr);
    client_socket =
        accept(port_sock->second, reinterpret_cast<sockaddr*>(&client_addr),
               &client_len);
    if (client_socket < 0) {
        return std::shared_ptr<ClientManager>(nullptr);
    }

    std::shared_ptr<ClientManager> res(new ClientManager(client_socket));
    client_managers_by_socket_.insert(std::make_pair(port_sock->second, res));
    return res;
}

}  // namespace TCP
