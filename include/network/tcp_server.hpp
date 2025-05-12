#include <string>
#include <asio.hpp>

class TCPServer {
private:
    int _port;
    asio::io_context _ioContext;
    asio::ip::tcp::acceptor _acceptor;

    void _accept();
public:
    TCPServer(int port);
    void start();
};