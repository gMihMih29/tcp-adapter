#pragma once

#include <cstdint>
#include <list>
#include <map>
#include <memory>

#include "ClientManager.hpp"

namespace TCP {

class Server {
public:
    ~Server();

    bool OpenPort(unsigned short port, int queue_limit);
    bool ClosePort(unsigned short port);

    std::shared_ptr<ClientManager> WaitForConnection(unsigned short port);

private:
    std::multimap<int, std::shared_ptr<ClientManager>> client_managers_by_socket_;
    std::map<unsigned short, int> sockets_by_port_;
};

}  // namespace TCP
