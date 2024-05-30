#pragma once

#include <stddef.h>
#include <sys/socket.h>

#include <vector>

namespace TCP {

class Client {
public:
    ~Client();

    bool Send(const char* buffer, size_t buffer_len, int flag = MSG_NOSIGNAL);
    int Recv(char* buffer, size_t buffer_len, int flag = MSG_NOSIGNAL);

    bool OpenConnection(const char* server_ip, unsigned short port);
    void CloseConnection();
    bool IsConnected() const;

private:
    bool is_connected_ = false;
    int socket_ = -1;
};

}  // namespace TCP
