#include "network/tcp_connection.hpp"
#include <asio.hpp>
#include "core/protocol/protocol.hpp"
#include "core/command/command.hpp"

using asio::ip::tcp;

tcp::socket& TCPConnection::socket() {
    return this->_socket;
}

TCPConnection::TCPConnection(asio::io_context &ioContext) :
    _socket(ioContext)
{}

int TCPConnection::userId() const {
    return this->_userId;
}

void TCPConnection::setUserId(int userId) {
    this->_userId = userId;
}

void TCPConnection::readMessage(std::function<std::unordered_map<std::string, Protocol::Value>(const Protocol::Packet&)> handleCommand) {
    auto self = shared_from_this();

    asio::async_read_until(
        this->socket(),
        this->streambuf(),
        '\n',
        [self, handleCommand](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                std::istream is(&self->streambuf());
                std::string message;
                std::getline(is, message);

                try {
                    Protocol::Packet packet = Protocol::decode(message);
                    auto response = handleCommand(packet);
                    Protocol::Packet responsePacket(packet.command, response);
                    std::string jsonString = Protocol::encode(responsePacket);
                    self->writeMessage(jsonString);
                } catch (const std::exception& e) {
                    std::cerr << "Error decoding message: " << e.what() << "\n";
                    return;
                }

                self->readMessage(handleCommand);
            } else {
                std::cerr << "Error reading message: " << error.message() << "\n";
                self->disconnect();
            }
        }
    );
}

void TCPConnection::writeMessage(const std::string& message) {
    auto self = shared_from_this();
    auto buffer = std::make_shared<std::string>(message + "\n");

    asio::async_write(
        this->socket(),
        asio::buffer(*buffer),
        [self, buffer](const asio::error_code& error, std::size_t /*bytes_transferred*/) {
            if (error) {
                std::cerr << "Error writing message: " << error.message() << "\n";
                self->disconnect();
            }
        }
    );
}

void TCPConnection::addOnDisconnectListener(std::function<void(std::shared_ptr<TCPConnection>)> callback) {
    this->_disconnectCallbacks.push_back(std::move(callback));
}

void TCPConnection::disconnect() {
    for (const auto& callback : this->_disconnectCallbacks) {
        callback(shared_from_this());
    }
    this->socket().close();
}