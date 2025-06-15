#include <asio.hpp>
#include <iostream>
#include "network/tcp_server.hpp"
#include "core/database/db_config.hpp"
#include "database/mysql/mysql_connection.hpp"
#include "core/fighting_app.hpp"
#include "memory"
#include "command/signup_command.hpp"
#include "command/signin_command.hpp"
#include "command/auth_command.hpp"
#include "core/protocol/protocol.hpp"
#include "service/user_service.hpp"
#include "service/auth_service.hpp"
#include "service/room_service.hpp"
#include "command/test_command.hpp"
#include "command/room_command.hpp"



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

    // Create the fighting game application
    auto fightingApp = std::make_unique<FightingGameApplication>();
    fightingApp
        ->registerDatabaseConnection(dbConnection)
        ->registerAuthMiddleware(new AuthCommand())
        ->registerService(new UserService())
        ->registerService(new AuthService())
        ->registerService(new RoomService())
        ->registerCommand(Protocol::Command::SIGN_UP, new SignupCommand(), true)
        ->registerCommand(Protocol::Command::SIGN_IN, new SigninCommand(), true)
        ->registerCommand(Protocol::Command::WAIT_FOR_ROOM, new RoomCommand(), false)
        ->registerCommand(Protocol::Command::TEST, new TestCommand(), false);

    std::cout << "Server is running on port " << PORT << "\n";
    // Run the server
    fightingApp->start(PORT);
    return 0;
}