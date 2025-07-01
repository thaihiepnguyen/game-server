#pragma once

#include <asio.hpp>
#include <functional>
#include "core/network/events/network_event_manager.hpp"
#include <iostream>

using asio::ip::tcp;

class TCPConnection
{
private:
    tcp::socket _socket;

public:
    tcp::socket &socket() { return _socket; }

    TCPConnection(asio::io_context &io_context);
    /**
     * Receives data from the socket asynchronously.
     * @param handler Function to handle the received data.
     */
    void recv(std::function<void(const char *, std::size_t)> handler);

    /**
     * Sends data to the socket asynchronously.
     * @param buffer Data to send.
     * @param length Length of the data to send.
     */
    void send(const char *buffer, std::size_t length);

public:
    /**
     * Event manager for handling network events.
     */
    NetworkEventManager events;

    /**
     * Disconnects the TCP connection.
     */
    void disconnect();

    bool isClosed();
};