#include <asio.hpp>
#include <iostream>
#include "network/tcp_server.hpp"

using asio::ip::tcp;

const int PORT = 12345;


int main() {
    TCPServer tcpServer(PORT);

    tcpServer.start();
    return 0;
}