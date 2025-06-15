#pragma once

#include <asio.hpp>
#include <functional>
#include "core/protocol/protocol.hpp"

using asio::ip::tcp;

class TCPConnection {
private:
    tcp::socket _socket;
    int _userId;
    asio::streambuf _streambuf;

public:
    tcp::socket& socket() { return _socket; }

    TCPConnection(asio::io_context &io_context);
    int userId() const;
    void setUserId(int userId);
    asio::streambuf& streambuf() { return _streambuf; }

    void recv(std::function<void(std::string)>);
    void send(const std::string& message);

    void disconnect();
};