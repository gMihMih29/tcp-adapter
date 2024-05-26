#pragma once

#include <stddef.h>

#include <vector>

namespace TCP {

class ServerClient {
public:
    ~ServerClient();

    bool Send(const std::vector<char>& buffer, size_t buffer_len = 0) const;
    bool Recv(std::vector<char>& buffer, size_t buffer_len = 0) const;

    bool OpenConnection(const char* server_ip, unsigned short port);
    void CloseConnection();

    bool IsConnected() const;

private:
    bool is_connected_ = false;
    int socket_ = 0;
};

}  // namespace TCP
