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
    asio::ip::tcp::acceptor _acceptor;
    std::vector<std::shared_ptr<TCPConnection>> _connections;

    void _accept();
    void _readMessage(std::shared_ptr<TCPConnection> connection);

    /**
     * Broadcasts a message to all connected clients except the source connection.
     * @param pos The position to broadcast.
     * @param connection The connection that sent the message.
     */
    void _broadcast(const Position& pos, std::shared_ptr<TCPConnection> sourceConnection);

    /**
     * Removes a connection from the server and closes the socket.
     * @param connection The connection to disconnect.
     */
    void _disconnect(std::shared_ptr<TCPConnection> connection);
public:
    TCPServer(int port);
    void start(void (*onServerStarted)());
};