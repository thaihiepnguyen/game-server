#pragma once

#include "network/tcp_connection.hpp"
#include "models/position.hpp"
#include <string>
#include <asio.hpp>
#include <vector>
#include <unordered_map>
#include "core/protocol/protocol.hpp"
#include <functional>

class TCPServer {
private:
    int _port;
    asio::io_context _ioContext;
    std::shared_ptr<asio::ip::tcp::acceptor> _acceptor;
public:
    TCPServer();
    /**
     * Establishes a port on which to listen.
     */
    bool bind(unsigned short&);

    /**
     * Checks for new client connections
     */
    bool listen();

    /**
     * Accept an attempt client connection
     */
    void accept(std::function<void(std::shared_ptr<TCPConnection>, std::string)>);
};