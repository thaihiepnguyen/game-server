#pragma once

#include "core/network/tcp_connection.hpp"
#include <string>
#include <asio.hpp>
#include <vector>
#include <unordered_map>
#include <functional>

class TCPServer
{
private:
    int _port;
    asio::io_context _ioContext;
    std::shared_ptr<asio::ip::tcp::acceptor> _acceptor;
    std::vector<std::shared_ptr<TCPConnection>> _connections;

public:
    TCPServer();
    /**
     * Establishes a port on which to listen.
     */
    bool bind(unsigned short &);

    /**
     * Checks for new client connections
     */
    bool listen();

    /**
     * Accept an attempt client connection
     * @param handler Function to handle the accepted connection.
     */
    void accept(std::function<void(std::shared_ptr<TCPConnection>, const char *, std::size_t)> handler);

    /**
     * Starts the server to listen for incoming connections.
     */
    void run();
};