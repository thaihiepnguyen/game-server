#include "command/auth_command.hpp"



void AuthCommand::execute(
    const std::shared_ptr<TCPConnection>& connection,
    Protocol::Packet& packet) 
{
    // Validate request format
    if (std::holds_alternative<std::monostate>(packet.data.at("act"))){
        throw std::runtime_error("Invalid request format: 'act' field is missing or empty");
    }

    std::string token = std::get<std::string>(packet.data.at("act"));
    if (token.empty()) {
        throw std::runtime_error("Invalid request format: 'act' field cannot be empty");
    }

    // Process the authentication action
    long long userId = _authService->verifyToken(token);

    std::unordered_map<std::string, Protocol::Value> res;
    res["user_id"] = Protocol::Value(userId);
    connection->setUserId(static_cast<int>(userId));
}