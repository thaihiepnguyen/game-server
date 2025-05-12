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
    while (true) {
        auto newConnection = std::make_shared<TCPConnection>(this->_ioContext);
         
        this->_acceptor.accept(newConnection->socket());
        std::cout << "New connection accepted.\n";

        std::string hello_message = "Hello from server!";
        asio::error_code error;
        asio::write(newConnection->socket(), asio::buffer(hello_message), error);
        if (error) {
            std::cerr << "Error sending message: " << error.message() << "\n";
        } else {
            std::cout << "Sent message: " << hello_message << "\n";
        }
    }
}