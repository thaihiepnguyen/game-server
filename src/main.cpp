#include <asio.hpp>
#include <iostream>
#include "network/tcp_server.hpp"
#include "database/db_config.hpp"
#include "database/mysql/mysql_connection.hpp"

using asio::ip::tcp;

const int PORT = 8080;
const std::string DB_JSON_PATH = "../configs/db.json";


int main() {
    TCPServer tcpServer;
    tcpServer.run(PORT, []() {        
        // Start the TCP server

        std::cout << "Server started on port " << PORT << "\n";
    });
    
    return 0;
}