#include "network/tcp_server.hpp"
#include "network/tcp_connection.hpp"
#include <iostream>
#include <memory>

using asio::ip::tcp;

TCPServer::TCPServer(int port) : 
    _port(port),
    _acceptor(tcp::acceptor(this->_ioContext, tcp::endpoint(tcp::v4(), port)))
{}

void TCPServer::start() {
    std::cout << "Server is running on port " << this->_port << "\n";
    try {
        this->_accept();
        this->_ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void TCPServer::_accept() {
    auto newConnection = std::make_shared<TCPConnection>(this->_ioContext);

    this->_acceptor.async_accept(newConnection->socket(), [this, newConnection](const asio::error_code& error) {
        if (!error) {
            std::cout << "New connection accepted.\n";
            this->_connections.push_back(newConnection);
            this->_readMessage(newConnection);
        } else {
            std::cerr << "Accept error: " << error.message() << "\n";
        }

        this->_accept();
    });
}

void TCPServer::_readMessage(std::shared_ptr<TCPConnection> connection) {
    auto buffer = std::make_shared<std::array<char, 1024>>();
    connection->socket().async_read_some(asio::buffer(*buffer), [this, connection, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            std::string message(buffer->data(), bytes_transferred);
            std::cout << "Received message: " << message << "\n";
 
            if (message == "EXIT") {
                std::cout << "Client disconnected.\n";
                // Remove the connection from the list of active connections
                this->_connections.erase(std::find(this->_connections.begin(), this->_connections.end(), connection));
                connection->socket().close();
                return;
            }
 
            this->_readMessage(connection);
        } else {
            std::cerr << "Error reading message: " << error.message() << "\n";
 
            // Remove the connection from the list of active connections
            this->_connections.erase(std::find(this->_connections.begin(), this->_connections.end(), connection));
            connection->socket().close();
        }
    });
}