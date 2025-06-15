#include "command/signup_command.hpp"
#include "models/user.hpp"

void SignupCommand::execute(
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


    auto [success, message] = this->_authService->registerUser(username, password);

    if (!success) {
        response["status"] = Protocol::Value("error");
        response["message"] = Protocol::Value(message);
        Protocol::Packet res(packet.command, response);
        connection->send(Protocol::encode(res));
    }
    response["status"] = Protocol::Value("success");
    response["message"] = Protocol::Value("User registered successfully");

    Protocol::Packet res(packet.command, response);
    connection->send(Protocol::encode(res));
}