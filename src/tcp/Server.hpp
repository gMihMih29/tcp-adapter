#pragma once

#include <cstdint>
#include <list>
#include <map>
#include <memory>

#include "Client.hpp"

namespace TCP {

class Server {
public:
    Server(int queue_limit);

    ~Server();

    int OpenPort(unsigned short port);
    int CloseSocket(int socket);

    std::shared_ptr<Client> WaitForConnection(int socket);

private:
    std::multimap<int, std::shared_ptr<Client>> sockets_;
    int queue_limit_;
};

}  // namespace TCP
