#include "Client.hpp"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cassert>

namespace TCP {

Client::~Client() { CloseConnection(); }

bool Client::Send(const char* buffer, size_t buffer_len) {
    if (!IsConnected()) {
        return false;
    }
    return send(socket_, buffer, buffer_len, send_flags_) == buffer_len;
}

int Client::Recv(char* buffer, size_t buffer_len) {
    if (!IsConnected()) {
        return 0;
    }
    int recv_size = recv(socket_, buffer, buffer_len, recv_flags_);
    if (recv_size == 0) {
        CloseConnection();
    }
    return recv_size;
}

bool Client::OpenConnection(const char* server_ip, unsigned short port) {
    CloseConnection();
    if ((socket_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return false;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(port);

    if (connect(socket_, reinterpret_cast<sockaddr*>(&serv_addr),
                sizeof(serv_addr)) < 0) {
        CloseConnection();
        return false;
    }
    is_connected_ = true;
    return true;
}

void Client::SetSendFlags(int flag) { send_flags_ = flag; }

void Client::SetRecvFlags(int flag) { recv_flags_ = flag; }

void Client::CloseConnection() {
    is_connected_ = false;
    close(socket_);
    socket_ = -1;
}

bool Client::IsConnected() const { return is_connected_; }

}  // namespace TCP
