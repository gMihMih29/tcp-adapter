#include "Client.hpp"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>

namespace TCP {

Client::Client(int socket) : is_connected_(true), socket_(socket) {
    assert(socket > 0 && "Cannot use socket with negative value");
}

Client::~Client() { CloseConnection(); }

bool Client::Send(const std::vector<char>& buffer, size_t buffer_len) {
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

bool Client::Recv(std::vector<char>& buffer, size_t buffer_len) {
    if (!IsConnected()) {
        return false;
    }
    if (buffer_len == 0) {
        buffer_len = buffer.size();
    }
    assert(buffer_len <= buffer.size() &&
           "buffer_len must be less or equal to length of buffer");
    assert(IsConnected() && "Connection to client must be open");
    int recv_size;
    if ((recv_size = recv(socket_, buffer.data(), buffer_len, 0)) < 0) {
        return false;
    }
    buffer.resize(recv_size);
    if (recv_size == 0) {
        CloseConnection();
    }
    return true;
}

void Client::CloseConnection() {
    is_connected_ = false;
    close(socket_);
}

bool Client::IsConnected() const { return is_connected_; }

}  // namespace TCP
