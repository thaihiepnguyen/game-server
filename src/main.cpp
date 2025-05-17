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

    DbConfig dbconfig(DB_JSON_PATH);
    auto conn = MysqlConnection::instance();

    conn->connect(dbconfig);

    if (!conn->isConnected()) {
        std::cerr << "Failed to connect to the database.\n";
        return 1;
    }
    std::cout << "Connected to the database.\n";

    auto res = conn->session()->sql("SELECT * FROM test_table").execute();
    // Handle the result of the query here
    
    // For example, you can iterate over the rows:
    for (auto row : res) {
        std::cout << "User ID: " << row[0].get<int>() << ", Name: " << row[1].get<std::string>() << "\n";
    }
    conn->disconnect(); // Disconnect from the database

    tcpServer.run(PORT, []() {        
        // Start the TCP server

        std::cout << "Server started on port " << PORT << "\n";
    });
    
    return 0;
}