#include "ServerClient.hpp"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>

namespace TCP {

ServerClient::~ServerClient() { CloseConnection(); }

bool ServerClient::Send(const std::vector<char>& buffer, size_t buffer_len) const {
    if (!IsConnected()) {
        return false;
    }
    if (buffer_len == 0) {
        buffer_len = buffer.size();
    }
    assert(buffer_len <= buffer.size() &&
           "buffer_len must be less or equal to length of buffer");
    if (send(socket_, buffer.data(), buffer_len, MSG_NOSIGNAL) != buffer_len) {
        return false;
    }
    return true;
}

bool ServerClient::Recv(std::vector<char>& buffer, size_t buffer_len) const {
    if (!IsConnected()) {
        return false;
    }
    if (buffer_len == 0) {
        buffer_len = buffer.capacity();
    }
    assert(buffer_len <= buffer.capacity() &&
           "buffer_len must be less or equal to length of buffer");
    assert(IsConnected() && "Connection to client must be open");
    int recv_size;
    if ((recv_size = recv(socket_, buffer.data(), buffer_len, 0)) < 0) {
        return false;
    }
    buffer.resize(recv_size);
    return true;
}

bool ServerClient::OpenConnection(const char* server_ip, unsigned short port) {
    CloseConnection();
    if ((socket_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return false;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(port);

    if (connect(socket_, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        CloseConnection();
        return false;
    }
    is_connected_ = true;
    return true;
}

void ServerClient::CloseConnection() {
    is_connected_ = false;
    close(socket_);
    socket_ = -1;
}

bool ServerClient::IsConnected() const { return is_connected_; }

}  // namespace TCP
