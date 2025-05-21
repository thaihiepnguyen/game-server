#include "network/tcp_connection.hpp"
#include <asio.hpp>
#include "protocol/protocol.hpp"
#include "service/command.hpp"
#include "service/signup_command.hpp"

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

void TCPConnection::readMessage() {
    auto self = shared_from_this();

    asio::async_read_until(
        this->socket(),
        this->streambuf(),
        '\n',
        [self](const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                std::istream is(&self->streambuf());
                std::string message;
                std::getline(is, message);

                try {
                    Protocol::Packet packet = Protocol::decode(message);
                    std::cout << "Received message: " << packet.toString() << "\n";
                    
                    // Handle the packet based on its command
                    if (packet.command == Protocol::Command::SIGN_UP) {
                        // Create a new command handler
                        std::shared_ptr<ICommand> handler = std::make_shared<SignupCommand>();
                        // Execute the command with the provided data
                        auto response = handler->execute(packet.data);
                        // Prepare the response packet
                        Protocol::Packet responsePacket(packet.command, response);
                        // Encode the response packet to JSON
                        std::string jsonString = Protocol::encode(responsePacket);
                        std::cout << "Sending response: " << jsonString << "\n";
                        self->writeMessage(jsonString);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error decoding message: " << e.what() << "\n";
                    return;
                }
                

                self->readMessage();
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