#pragma once

#include <stddef.h>
#include <sys/socket.h>

#include <vector>

namespace TCP {

class ClientManager {
public:
    ClientManager(int socket);
    ~ClientManager();

    bool Send(const char* buffer, size_t buffer_len);
    int Recv(char* buffer, size_t buffer_len);

    void SetSendFlags(int flag);
    void SetRecvFlags(int flag);

    void CloseConnection();
    bool IsConnected() const;

private:
    bool is_connected_;
    int socket_;
    int send_flags_ = MSG_NOSIGNAL;
    int recv_flags_ = MSG_NOSIGNAL;
};

}  // namespace TCP
