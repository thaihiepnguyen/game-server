#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <memory>

class DbConfig {
private:
    std::string _host;
    std::string _user;
    std::string _password;
    std::string _database;
    int _port;
public:
    DbConfig()
        : _host(""),
        _user(""),
        _password(""),
        _database(""),
        _port(3306) {}
    
    DbConfig(const std::string& filename);
    void loadFromFile(const std::string& filename);
    std::string host() const { return _host; }
    std::string user() const { return _user; }
    std::string password() const { return _password; }
    std::string database() const { return _database; }
    int port() const { return _port; }

    std::string toString() const;
};
