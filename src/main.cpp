#include <asio.hpp>
#include <iostream>
#include "network/tcp_server.hpp"
#include "database/db_config.hpp"

using asio::ip::tcp;

const int PORT = 8080;
const std::string DB_JSON_PATH = "../configs/db.json";


int main() {
    TCPServer tcpServer(PORT);

    tcpServer.start([]() {
        DbConfig dbconfig(DB_JSON_PATH);

        std::cout << dbconfig.toString() << "\n";
        std::cout << "Server started on port " << PORT << "\n";
    });
    
    return 0;
}