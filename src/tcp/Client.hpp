#pragma once

#include <stddef.h>

#include <vector>

namespace TCP {

class Client {
public:
    Client(int socket);
    ~Client();

    bool Send(const std::vector<char>& buffer, size_t buffer_len = 0);
    bool Recv(std::vector<char>& buffer, size_t buffer_len = 0);

    void CloseConnection();

    bool IsConnected() const;

private:
    bool is_connected_;
    int socket_;
};

}  // namespace TCP
