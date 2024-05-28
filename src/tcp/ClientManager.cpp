#include "ClientManager.hpp"

#include <arpa/inet.h>
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

bool ClientManager::Send(const char* buffer, size_t buffer_len) {
    if (!IsConnected()) {
        return false;
    }
    return send(socket_, buffer, buffer_len, send_flags_) == buffer_len;
}

int ClientManager::Recv(char* buffer, size_t buffer_len) {
    if (!IsConnected()) {
        return 0;
    }
    int recv_size = recv(socket_, buffer, buffer_len, recv_flags_);
    if (recv_size == 0) {
        CloseConnection();
    }
    return recv_size;
}

void ClientManager::SetSendFlags(int flag) { send_flags_ = flag; }

void ClientManager::SetRecvFlags(int flag) { recv_flags_ = flag; }

void ClientManager::CloseConnection() {
    is_connected_ = false;
    close(socket_);
    socket_ = -1;
}

bool ClientManager::IsConnected() const { return is_connected_; }

}  // namespace TCP
