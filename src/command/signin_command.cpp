#include "command/signin_command.hpp"

void SigninCommand::execute(
    const std::shared_ptr<TCPConnection>& connection,
    Protocol::Packet& packet) 
{
    // validate request format
    std::unordered_map<std::string, Protocol::Value> response;
    if (std::holds_alternative<std::monostate>(packet.data.at("username")) || 
        std::holds_alternative<std::monostate>(packet.data.at("password"))) {
        response["status"] = Protocol::Value("error");
        response["message"] = Protocol::Value("Invalid request format");
        
        Protocol::Packet res(packet.command, response);
        connection->send(Protocol::encode(res));
    }
    std::string username = std::get<std::string>(packet.data.at("username"));
    std::string password = std::get<std::string>(packet.data.at("password"));

    if (username.empty() || password.empty()) {
        response["status"] = Protocol::Value("error");
        response["message"] = Protocol::Value("Username and password cannot be empty");
        Protocol::Packet res(packet.command, response);
        connection->send(Protocol::encode(res));
    }

    auto [success, message, token] = this->_authService->loginUser(username, password);
    if (!success) {
        response["status"] = Protocol::Value("error");
        response["message"] = Protocol::Value(message);
        Protocol::Packet res(packet.command, response);
        connection->send(Protocol::encode(res));
    }


    response["status"] = Protocol::Value("success");
    response["message"] = Protocol::Value("User login successfully");
    response["token"] = Protocol::Value(token);
    Protocol::Packet res(packet.command, response);
    connection->send(Protocol::encode(res));
}