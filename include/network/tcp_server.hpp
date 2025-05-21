#pragma once

#include "network/tcp_connection.hpp"
#include "models/position.hpp"
#include <string>
#include <asio.hpp>
#include <vector>

class TCPServer {
private:
    int _port;
    asio::io_context _ioContext;
    std::shared_ptr<asio::ip::tcp::acceptor> _acceptor;
    std::vector<std::shared_ptr<TCPConnection>> _connections;

    void _accept();

    /**
     * Broadcasts a message to all connected clients except the source connection.
     * @param pos The position to broadcast.
     * @param connection The connection that sent the message.
     */
    void _broadcast(const Position& pos, std::shared_ptr<TCPConnection> sourceConnection);
public:
    TCPServer();
    void run(int port, void (*onServerStarted)());
};