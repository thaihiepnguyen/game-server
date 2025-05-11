#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

void handle_client(tcp::socket socket) {
    try {
        char data[512];
        while (true) {
            std::size_t length = socket.read_some(asio::buffer(data));
            std::cout << "Received: " << std::string(data, length) << std::endl;
            asio::write(socket, asio::buffer(data, length)); // Echo message back to client
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "Server listening on port 12345..." << std::endl;

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Client connected!" << std::endl;
            handle_client(std::move(socket));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}