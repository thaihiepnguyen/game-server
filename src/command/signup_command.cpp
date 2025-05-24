#include "command/signup_command.hpp"

std::unordered_map<std::string, Protocol::Value> SignupCommand::execute(
const std::unordered_map<std::string, Protocol::Value>& request
) {
    std::unordered_map<std::string, Protocol::Value> response;
    std::string username = std::get<std::string>(request.at("username"));
    std::string password = std::get<std::string>(request.at("password"));

    auto [salt, hashedPassword] = _authService->hashPassword(password);

    _userService->createUser(username, hashedPassword, salt);

    response["status"] = Protocol::Value("success");
    response["message"] = Protocol::Value("User created successfully");
    return response;
}