#include "core/network/tcp_server.hpp"
#include "core/network/tcp_connection.hpp"
#include <iostream>
#include <memory>

using asio::ip::tcp;

TCPServer::TCPServer()
    : _acceptor(NULL), _port(0)
{
}

bool TCPServer::bind(unsigned short &port)
{
    this->_port = port;
    try
    {
        this->_acceptor = std::make_shared<tcp::acceptor>(this->_ioContext);
        tcp::endpoint endpoint(tcp::v4(), this->_port);

        this->_acceptor->open(endpoint.protocol());
        this->_acceptor->bind(endpoint);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Bind error: " << e.what() << std::endl;
        return false;
    }
}

bool TCPServer::listen()
{
    try
    {
        _acceptor->listen(asio::socket_base::max_listen_connections);
        return true;
    }
    catch (const asio::system_error &error)
    {
        std::cerr << "Listen error: " << error.what() << std::endl;
        return false;
    }
}

void TCPServer::accept(std::function<void(std::shared_ptr<TCPConnection>, const char *, std::size_t)> handler)
{
    auto connection = std::make_shared<TCPConnection>(this->_ioContext);

    this->_acceptor->async_accept(
        connection->socket(),
        [this, connection, handler](const asio::error_code &error)
        {
            if (!error)
            {
                _connections.push_back(connection);
                std::weak_ptr<TCPConnection> weakConn = connection;
                connection->events.subscribe("disconnect",
                                             [this, weakConn]()
                                             {
                                                 if (auto conn = weakConn.lock())
                                                 {
                                                     auto it = std::remove(_connections.begin(), _connections.end(), conn);
                                                     if (it != _connections.end())
                                                     {
                                                         _connections.erase(it);
                                                     }
                                                 }
                                             });
                connection->recv([connection, handler](const char *data, std::size_t length)
                                 { handler(connection, data, length); });
            }
            else
            {
                std::cerr << "Accept error: " << error.message() << std::endl;
            }

            this->accept(handler);
        });
}

void TCPServer::run()
{
    try
    {
        this->_ioContext.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Run error: " << e.what() << std::endl;
    }
}