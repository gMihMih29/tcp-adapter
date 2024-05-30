#pragma once

#include <stddef.h>
#include <sys/socket.h>

#include <vector>

namespace TCP {

class ClientManager {
public:
    ClientManager(int socket);
    ~ClientManager();

    bool Send(const char* buffer, size_t buffer_len, int flag = MSG_NOSIGNAL);
    int Recv(char* buffer, size_t buffer_len, int flag = MSG_NOSIGNAL);

    void CloseConnection();
    bool IsConnected() const;

private:
    bool is_connected_;
    int socket_;
};

}  // namespace TCP
