#include <asio.hpp>
#include <iostream>
#include "network/tcp_server.hpp"
#include "core/database/db_config.hpp"
#include "database/mysql/mysql_connection.hpp"
#include "core/mmorpg_app.hpp"
#include "memory"
#include "command/signup_command.hpp"
#include "command/auth_command.hpp"
#include "core/protocol/protocol.hpp"
#include "service/user_service.hpp"
#include "service/auth_service.hpp"



using asio::ip::tcp;

const unsigned short PORT = 8080;
const std::string DB_JSON_PATH = "../configs/db.json";


int main() {
    // Load database configuration
    DbConfig dbConfig(DB_JSON_PATH);
    IDatabaseConnection* dbConnection = new MysqlConnection();
    if (!dbConnection->connect(dbConfig)) {
        std::cerr << "Failed to connect to the database.\n";
        delete dbConnection;
        return 1;
    }

    // Create the MMORPG application
    MMORPGApplication* mmorpgApp = new MMORPGApplication();
    mmorpgApp
        ->registerDatabaseConnection(dbConnection)
        ->registerAuthMiddleware(new AuthCommand())
        ->registerService(new AuthService())
        ->registerService(new UserService())
        ->registerCommand(Protocol::Command::SIGN_UP, new SignupCommand(), true);

    std::cout << "Server is running on port " << PORT << "\n";
    // Run the server
    mmorpgApp->listen(PORT);
    
    // Clean up
    delete mmorpgApp;
    return 0;
}