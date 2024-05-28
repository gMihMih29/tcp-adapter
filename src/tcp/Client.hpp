#pragma once

#include <stddef.h>
#include <sys/socket.h>

#include <vector>

namespace TCP {

class Client {
public:
    ~Client();

    bool Send(const char* buffer, size_t buffer_len);
    int Recv(char* buffer, size_t buffer_len);

    void SetSendFlags(int flag);
    void SetRecvFlags(int flag);

    bool OpenConnection(const char* server_ip, unsigned short port);
    void CloseConnection();
    bool IsConnected() const;

private:
    bool is_connected_ = false;
    int socket_ = 0;
    int send_flags_ = MSG_NOSIGNAL;
    int recv_flags_ = MSG_NOSIGNAL;
};

}  // namespace TCP
