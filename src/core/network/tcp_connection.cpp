#include "core/network/tcp_connection.hpp"
#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

TCPConnection::TCPConnection(asio::io_context &ioContext) : _socket(ioContext)
{
}

void TCPConnection::recv(std::function<void(const char *, std::size_t)> handler)
{
    if (this->isClosed())
    {
        std::cerr << "Error: Cannot receive data, socket is closed.\n";
        return;
    }
    auto buffer = std::make_shared<std::array<char, 1024>>();
    this->_socket.async_read_some(
        asio::buffer(*buffer),
        [this, buffer, handler](const asio::error_code &error, std::size_t length)
        {
            if (!error)
            {
                handler(buffer->data(), length);
                recv(handler); // Continue receiving data
            }
            else
            {
                this->disconnect();
            }
        });
}

void TCPConnection::send(const char *buffer, std::size_t length)
{
    if (length == 0 || buffer == nullptr)
    {
        std::cerr << "Error: Attempted to send an empty buffer.\n";
        return;
    }

    if (this->isClosed())
    {
        std::cerr << "Error: Cannot send data, socket is closed.\n";
        return;
    }

    asio::async_write(
        this->socket(),
        asio::buffer(buffer, length),
        [this](const asio::error_code &error, std::size_t /*bytes_transferred*/)
        {
            if (error)
            {
                std::cerr << "Error writing message: " << error.message() << "\n";
                this->disconnect();
            }
        });
}

void TCPConnection::disconnect()
{
    this->socket().close();
    this->events.notify("disconnect");
}

bool TCPConnection::isClosed()
{
    return !this->socket().is_open();
}