#include "network/tcp_server.hpp"
#include "network/tcp_connection.hpp"
#include "models/position.hpp"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>


using asio::ip::tcp;

TCPServer::TCPServer()
: _acceptor(NULL), _port(0)
{}

void TCPServer::run(int port, void (*onServerStarted)()) 
{
    this->_port = port;
    this->_acceptor = std::make_shared<tcp::acceptor>(this->_ioContext, tcp::endpoint(tcp::v4(), this->_port));
    try {
        this->_accept();
        if (onServerStarted) {
            onServerStarted();
        }
        this->_ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void TCPServer::_disconnect(std::shared_ptr<TCPConnection> connection)
{
    auto it = std::find(this->_connections.begin(), this->_connections.end(), connection);
    if (it != this->_connections.end()) {
        this->_connections.erase(it);
        connection->socket().close();
    }
}

void TCPServer::_broadcast(const Position &pos, std::shared_ptr<TCPConnection> sourceConnection)
{
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
                        this->_disconnect(destConnection);
                    }
                }
            );
        }
    }
}

void TCPServer::_accept() {
    auto newConnection = std::make_shared<TCPConnection>(this->_ioContext);

    this->_acceptor->async_accept(newConnection->socket(),
        [this, newConnection](const asio::error_code& error) {
            if (!error) {
                std::cout << "New connection accepted.\n";
                this->_connections.push_back(newConnection);
                this->_readMessage(newConnection);
            } else {
                std::cerr << "Error accepting connection: " << error.message() << "\n";
            }
            this->_accept();
        });
}

void TCPServer::_readMessage(std::shared_ptr<TCPConnection> connection) {
    auto buffer = std::make_shared<std::array<char, 1024>>();

    connection->socket().async_read_some(asio::buffer(*buffer), [this, connection, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            std::string message(buffer->data(), bytes_transferred);
            try {
                nlohmann::json jsonMessage = nlohmann::json::parse(message);
                if (jsonMessage.contains("playerId")) {
                    connection->setUserId(jsonMessage["playerId"].get<int>());
                }
                std::string command = jsonMessage["command"].get<std::string>();
                
                if (command == "EXIT") {
                    std::cout << "Client " << connection->userId() << " disconnected.\n";
                    this->_disconnect(connection);
                } else {
                    auto position = jsonMessage["position"].get<Position>();
                    this->_broadcast(position, connection);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }

            this->_readMessage(connection);
        } else {
            std::cerr << "Error reading message: " << error.message() << "\n";
            this->_disconnect(connection);
        }
    });

}