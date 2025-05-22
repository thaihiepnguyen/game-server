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

void TCPServer::_broadcast(const Position &pos, std::shared_ptr<TCPConnection> sourceConnection) {
    for (const auto& destConnection : this->_connections) {
        if (destConnection != sourceConnection) {
            nlohmann::json jsonMessage;
            jsonMessage["command"] = "UPDATE_POSITION";
            jsonMessage["position"] = pos;
            jsonMessage["playerId"] = sourceConnection->userId();

            std::string message = jsonMessage.dump();
            auto buffer = std::make_shared<std::string>(message);
            asio::async_write(
                destConnection->socket(),
                asio::buffer(*buffer),
                [this, buffer, destConnection](const asio::error_code& ec, std::size_t /*bytes_transferred*/) {
                    if (ec) {
                        std::cerr << "Broadcast failed to client: " << ec.message() << "\n";
                    }
                }
            );
        }
    }
}

void TCPServer::_accept(std::function<std::unordered_map<std::string, Protocol::Value>(const Protocol::Packet&)> handleCommand) {
    auto newConnection = std::make_shared<TCPConnection>(this->_ioContext);

    this->_acceptor->async_accept(newConnection->socket(),
        [this, newConnection, handleCommand](const asio::error_code& error) {
            if (!error) {
                std::cout << "New connection accepted.\n";
                this->_connections.push_back(newConnection);
                newConnection->addOnDisconnectListener([this](std::shared_ptr<TCPConnection> connection) {
                    this->_connections.erase(std::remove(this->_connections.begin(), this->_connections.end(), connection), this->_connections.end());
                });

                newConnection->readMessage(handleCommand);
            } else {
                std::cerr << "Error accepting connection: " << error.message() << "\n";
            }
            this->_accept(handleCommand);
        });
}