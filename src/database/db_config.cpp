#include "database/db_config.hpp"
#include <fstream>



DbConfig::DbConfig(const std::string &filename)
{
    this->loadFromFile(filename);
}

void DbConfig::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + filename);
    }

    nlohmann::json jsonConfig = nlohmann::json::parse(file);
    if (jsonConfig.is_null()) {
        throw std::runtime_error("Invalid JSON in config file: " + filename);
    }

    const std::vector<std::pair<std::string, std::function<void()>>> requiredFields = {
        {"host",     [&]() { this->_host     = jsonConfig["host"].get<std::string>(); }},
        {"user",     [&]() { this->_user     = jsonConfig["user"].get<std::string>(); }},
        {"password", [&]() { this->_password = jsonConfig["password"].get<std::string>(); }},
        {"database", [&]() { this->_database = jsonConfig["database"].get<std::string>(); }},
        {"port",     [&]() { this->_port     = jsonConfig["port"].get<int>(); }}
    };
    
    for (const auto& [key, setter] : requiredFields) {
        if (jsonConfig.contains(key)) {
            setter();
        } else {
            throw std::runtime_error("Missing '" + key + "' in config file: " + filename);
        }
    }
}

std::string DbConfig::toString() const
{
    return "DBConfig: { host: " + _host + ", user: " + _user + ", password: " + _password +
            ", database: " + _database + ", port: " + std::to_string(_port) + " }";
}