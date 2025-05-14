#pragma once

#include <asio.hpp>

using asio::ip::tcp;

class TCPConnection {
private:
    tcp::socket _socket;

public:
    tcp::socket& socket();
    TCPConnection(asio::io_context &io_context);
};