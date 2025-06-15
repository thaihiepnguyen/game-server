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

bool TCPServer::bind(unsigned short& port) {
    this->_port = port;
    try {
        this->_acceptor = std::make_shared<tcp::acceptor>(this->_ioContext, tcp::endpoint(tcp::v4(), this->_port));
        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

bool TCPServer::listen() {
    try {
        _acceptor->listen(asio::socket_base::max_listen_connections);
        return true;
    } catch (const asio::system_error& error) {
        std::cerr << "Listen error: " << error.what() << std::endl;
        return false;
    }
}

void TCPServer::accept(std::function<void(std::shared_ptr<TCPConnection>, std::string)> handler) {
    auto connection = std::make_shared<TCPConnection>(this->_ioContext);

    this->_acceptor->async_accept(
        connection->socket(),
        [&](const asio::error_code& error) {
            if (!error) {
                connection->recv([connection, handler](std::string msg) {
                    handler(connection, msg);
                });
            } else {
                std::cerr << "Accept error: " << error.message() << std::endl;
            }


            this->accept(handler);
        }
    );
}