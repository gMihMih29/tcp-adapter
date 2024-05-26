#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "tcp-adapter.hpp"

int main() {
    TCP::ServerClient clnt;
    if (!clnt.OpenConnection("127.0.0.1", 8554)) {
        return 0;
    }
    std::cout << "Connected to server!\n";
    for (int j = 0; j < 5; ++j) {
        std::string input = "hello";
        std::vector<char> buf(input.size());
        for (int i = 0; i < input.size(); ++i) {
            buf[i] = input[i];
        }
        if (!clnt.Send(buf, buf.size())) {
            std::cout << "Cannot send message\n";
            break;
        }
        if (!clnt.Recv(buf, buf.size())) {
            std::cout << "Cannot recv message\n";
            break;
        }
        std::cout << "\nReceived:\n";
        std::cout << buf.data() << "\n";
        sleep(2);
    }
    clnt.CloseConnection();
    return 0;
}
