#include "command/signup_command.hpp"

std::unordered_map<std::string, Protocol::Value> SignupCommand::execute(const std::unordered_map<std::string, Protocol::Value>& request) {
    std::unordered_map<std::string, Protocol::Value> response;
    if (std::holds_alternative<std::monostate>(request.at("username"))) {
        response["status"] = "error";
        response["message"] = "Username is required.";
        return response;
    }
    std::string username = std::get<std::string>(request.at("username"));
    std::string password = std::get<std::string>(request.at("password"));
    std::unordered_map<std::string, Protocol::Value> userData = std::get<std::unordered_map<std::string, Protocol::Value>>(request.at("position"));

    response["status"] = "success";
    return response;
}