#include "ClientManager.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>

namespace TCP {

ClientManager::ClientManager(int socket)
    : is_connected_(true), socket_(socket) {
    assert(socket > 0 && "Cannot use socket with negative value");
}

ClientManager::~ClientManager() { CloseConnection(); }

bool ClientManager::Send(const char* buffer, size_t buffer_len, int flag) {
    if (!IsConnected()) {
        return false;
    }
    bool res = (send(socket_, buffer, buffer_len, flag) == buffer_len);
    if (!res) {
        if (errno == EBADF) {
            CloseConnection();
        } else if (errno == EDESTADDRREQ) {
            CloseConnection();
        } else if (errno == ECONNRESET) {
            CloseConnection();
        } else if (errno == EFAULT) {
            CloseConnection();
        } else if (errno == ENOTCONN) {
            CloseConnection();
        } else if (errno == ENOTSOCK) {
            CloseConnection();
        }
    }
    return res;
}

int ClientManager::Recv(char* buffer, size_t buffer_len, int flag) {
    if (!IsConnected()) {
        return 0;
    }
    int recv_size = recv(socket_, buffer, buffer_len, flag);
    if (recv_size == 0) {
        CloseConnection();
    }
    if (recv_size < 0) {
        if (errno == EBADF) {
            CloseConnection();
        } else if (errno == ECONNREFUSED) {
            CloseConnection();
        } else if (errno == ENOTCONN) {
            CloseConnection();
        } else if (errno == ENOTSOCK) {
            CloseConnection();
        }
    }
    return recv_size;
}

void ClientManager::CloseConnection() {
    is_connected_ = false;
    close(socket_);
    socket_ = -1;
}

bool ClientManager::IsConnected() const { return is_connected_; }

}  // namespace TCP
