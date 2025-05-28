#include "command/auth_command.hpp"



std::unordered_map<std::string, Protocol::Value> AuthCommand::execute(
    const std::shared_ptr<TCPConnection>& connection,
    const std::unordered_map<std::string, Protocol::Value>& request) 
{
    auto response = request;
    // Validate request format
    if (std::holds_alternative<std::monostate>(request.at("act"))){
        throw std::runtime_error("Invalid request format: 'act' field is missing or empty");
    }

    std::string token = std::get<std::string>(request.at("act"));
    if (token.empty()) {
        throw std::runtime_error("Invalid request format: 'act' field cannot be empty");
    }

    // Process the authentication action
    long long userId = _authService->verifyToken(token);

    response["user_id"] = Protocol::Value(userId);
    connection->setUserId(static_cast<int>(userId));

    return response;
}