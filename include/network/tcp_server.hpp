#pragma once

#include "network/tcp_connection.hpp"
#include "models/position.hpp"
#include <string>
#include <asio.hpp>
#include <vector>
#include <unordered_map>
#include "core/protocol/protocol.hpp"

class TCPServer {
private:
    int _port;
    asio::io_context _ioContext;
    std::shared_ptr<asio::ip::tcp::acceptor> _acceptor;
    std::function<void(std::shared_ptr<TCPConnection>)> _newConnectionCallbacks;
    std::function<void(std::shared_ptr<TCPConnection>)> _disconnectionCallbacks;

    void _accept(std::function<std::unordered_map<std::string, Protocol::Value>(
        const std::shared_ptr<TCPConnection>& connection,
        const Protocol::Packet&)> handleCommand);

public:
    TCPServer();
    void run(int port, std::function<std::unordered_map<std::string, Protocol::Value>(
        const std::shared_ptr<TCPConnection>& connection,
        const Protocol::Packet&)> handleCommand);
    
    /**
     * Notifies the server about a new connection.
     * @param connection The new TCP connection.
     */
    void addOnNewConnectionListener(std::function<void(std::shared_ptr<TCPConnection>)> callback) {
        _newConnectionCallbacks = callback;
    }

    /**
     * Notifies the server about a disconnection.
     * @param connection The TCP connection that was disconnected.
     */
    void addOnDisconnectListener(std::function<void(std::shared_ptr<TCPConnection>)> callback) {
        _disconnectionCallbacks = callback;
    }
};