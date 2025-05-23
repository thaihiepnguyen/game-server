#include "command/auth_command.hpp"



std::unordered_map<std::string, Protocol::Value> AuthCommand::execute(const std::unordered_map<std::string, Protocol::Value>& request) {
    std::cout << "Executing AuthCommand...\n";
    return request;
}