#include "network/tcp_server.hpp"
#include "network/tcp_connection.hpp"
#include "models/position.hpp"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include "core/protocol/protocol.hpp"


using asio::ip::tcp;

TCPServer::TCPServer()
: _acceptor(NULL), _port(0)
{}

void TCPServer::run(int port, std::function<std::unordered_map<std::string, Protocol::Value>(const Protocol::Packet&)> handleCommand) {
    this->_port = port;
    this->_acceptor = std::make_shared<tcp::acceptor>(this->_ioContext, tcp::endpoint(tcp::v4(), this->_port));
    try {
        this->_accept(handleCommand);
        this->_ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void TCPServer::_accept(std::function<std::unordered_map<std::string, Protocol::Value>(const Protocol::Packet&)> handleCommand) {
    auto newConnection = std::make_shared<TCPConnection>(this->_ioContext);

    this->_acceptor->async_accept(newConnection->socket(),
        [this, newConnection, handleCommand](const asio::error_code& error) {
            if (!error) {
                // Notify about the new connection
                if (this->_newConnectionCallbacks) {
                    this->_newConnectionCallbacks(newConnection);
                }
                newConnection->addOnDisconnectListener([this](std::shared_ptr<TCPConnection> connection) {
                    // Notify about disconnection
                    if (this->_disconnectionCallbacks) {
                        this->_disconnectionCallbacks(connection);
                    }
                });

                newConnection->readMessage(handleCommand);
            } else {
                std::cerr << "Error accepting connection: " << error.message() << "\n";
            }
            this->_accept(handleCommand);
        });
}