#include <asio.hpp>
#include <iostream>
#include "network/tcp_server.hpp"
#include "core/database/db_config.hpp"
#include "database/mysql/mysql_connection.hpp"
#include "core/mmorpg_app.hpp"
#include "memory"
#include "service/signup_command.hpp"
#include "core/protocol/protocol.hpp"


using asio::ip::tcp;

const unsigned short PORT = 8080;
const std::string DB_JSON_PATH = "../configs/db.json";


int main() {
    // Load database configuration
    DbConfig dbConfig(DB_JSON_PATH);
    IDatabaseConnection* dbConnection = new MysqlConnection();
    if (!dbConnection->connect(dbConfig)) {
        std::cerr << "Failed to connect to the database.\n";
        return 1;
    }


    auto mmorpgApp = std::make_shared<MMORPGApplication>(PORT);
    mmorpgApp
        ->registerDatabaseConnection(dbConnection)
        ->registerCommand(Protocol::Command::SIGN_UP, new SignupCommand(), true);


    TCPServer tcpServer;
    tcpServer.run(PORT, []() {        
        // Start the TCP server

        std::cout << "Server started on port " << PORT << "\n";
    });
    
    return 0;
}