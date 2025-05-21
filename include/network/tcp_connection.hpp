#pragma once

#include <asio.hpp>
#include <functional>

using asio::ip::tcp;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
private:
    tcp::socket _socket;
    int _userId;
    asio::streambuf _streambuf;
    std::vector<std::function<void(std::shared_ptr<TCPConnection>)>> _disconnectCallbacks;

public:
    tcp::socket& socket();
    TCPConnection(asio::io_context &io_context);
    int userId() const;
    void setUserId(int userId);
    asio::streambuf& streambuf() { return _streambuf; }
    void readMessage();
    void writeMessage(const std::string& message);
    void addOnDisconnectListener(std::function<void(std::shared_ptr<TCPConnection>)> callback);
    void disconnect();
};