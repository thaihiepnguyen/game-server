#include "network/tcp_connection.hpp"
#include <asio.hpp>
#include "core/protocol/protocol.hpp"
#include "core/command/command.hpp"

using asio::ip::tcp;

TCPConnection::TCPConnection(asio::io_context &ioContext) :
    _socket(ioContext)
{}

int TCPConnection::userId() const {
    return this->_userId;
}

void TCPConnection::setUserId(int userId) {
    this->_userId = userId;
}

void TCPConnection::recv(std::function<void(std::string)> handler) {
    asio::async_read_until(
        this->_socket,
        this->streambuf(),
        '\n',
        [this, handler](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                std::istream is(&this->streambuf());
                std::string message;
                std::getline(is, message);

                handler(message);

                this->recv(handler);
            } else {
                std::cerr << "Error reading message: " << error.message() << "\n";
                this->disconnect();
            }
        }
    );
}

void TCPConnection::send(const std::string& message) {
    auto buffer = std::make_shared<std::string>(message + "\n");

    asio::async_write(
        this->socket(),
        asio::buffer(*buffer),
        [this, buffer](const asio::error_code& error, std::size_t /*bytes_transferred*/) {
            if (error) {
                std::cerr << "Error writing message: " << error.message() << "\n";
                this->disconnect();
            }
        }
    );
}

void TCPConnection::disconnect() {
    this->socket().close();
}