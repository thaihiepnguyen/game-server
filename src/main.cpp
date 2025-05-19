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

    try {
        auto res = conn->execute("SELECT * FROM test_table");
        std::cout << "Executed query on the database.\n";

        // Process the result
        for (const auto& row : res.rows()) {
            for (const auto& field : row) {
                std::cout << std::get<std::string>(*field) << " ";
            }
            std::cout << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error executing query: " << e.what() << "\n";
    }

    tcpServer.run(PORT, []() {        
        // Start the TCP server

        std::cout << "Server started on port " << PORT << "\n";
    });
    
    return 0;
}