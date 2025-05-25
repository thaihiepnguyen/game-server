#include "command/signin_command.hpp"

std::unordered_map<std::string, Protocol::Value> SigninCommand::execute(
const std::unordered_map<std::string, Protocol::Value>& request
) {
    std::unordered_map<std::string, Protocol::Value> response;
    if (std::holds_alternative<std::monostate>(request.at("username")) || 
        std::holds_alternative<std::monostate>(request.at("password"))) {
        response["status"] = Protocol::Value("error");
        response["message"] = Protocol::Value("Invalid request format");
        return response;
    }
    std::string username = std::get<std::string>(request.at("username"));
    std::string password = std::get<std::string>(request.at("password"));

    if (username.empty() || password.empty()) {
        response["status"] = Protocol::Value("error");
        response["message"] = Protocol::Value("Username and password cannot be empty");
        return response;
    }

    auto [salt, hashedPassword] = _authService->hashPassword(password);


    response["status"] = Protocol::Value("success");
    response["message"] = Protocol::Value("User created successfully");
    return response;
}