#pragma once

#include <string>
#include <asio.hpp>
#include <vector>
#include "network/tcp_connection.hpp"

class TCPServer {
private:
    int _port;
    asio::io_context _ioContext;
    asio::ip::tcp::acceptor _acceptor;
    std::vector<std::shared_ptr<TCPConnection>> _connections;

    void _accept();
    void _readMessage(std::shared_ptr<TCPConnection> connection);
public:
    TCPServer(int port);
    void start();
};