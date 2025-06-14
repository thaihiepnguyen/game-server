#include "command/signup_command.hpp"
#include "models/user.hpp"

std::unordered_map<std::string, Protocol::Value> SignupCommand::execute(
    const std::shared_ptr<TCPConnection>& connection,
    const std::unordered_map<std::string, Protocol::Value>& request)
{
    // validate request format
    std::unordered_map<std::string, Protocol::Value> response;
    if (std::holds_alternative<std::monostate>(request.at("username")) || 
        std::holds_alternative<std::monostate>(request.at("password"))) {
        response["status"] = Protocol::Value("error");
        response["message"] = Protocol::Value("Invalid request format");
        return response;
    }
    std::string username = std::get<std::string>(request.at("username"));
    std::string password = std::get<std::string>(request.at("password"));


    auto [success, message] = this->_authService->registerUser(username, password);

    if (!success) {
        response["status"] = Protocol::Value("error");
        response["message"] = Protocol::Value(message);
        return response;
    }
    response["status"] = Protocol::Value("success");
    response["message"] = Protocol::Value("User registered successfully");

    return response;
}