#include "network/tcp_connection.hpp"

using asio::ip::tcp;

tcp::socket& TCPConnection::socket() {
    return this->_socket;
}

TCPConnection::TCPConnection(asio::io_context &ioContext) :
    _socket(ioContext)
{}