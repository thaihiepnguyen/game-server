#pragma once

#include "database/db_connection.hpp"
#include <mysqlx/xdevapi.h>
#include <memory>
#include <iostream>
#include <stdexcept>

class MysqlConnection : public DBConnection<mysqlx::Session> {
private:
    std::shared_ptr<mysqlx::Session> _db;

public:
    MysqlConnection(const DbConfig& config) : DBConnection(config), _db(NULL) {}
    ~MysqlConnection() override {
        disconnect();
    }

    bool connect() override {
        try {
            this->_db = std::make_shared<mysqlx::Session>(_config.host(), _config.port(), _config.user(), _config.password());
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            this->_connected = false;
            return false;
        }
        this->_connected = true;
        return true;
    }

    void disconnect() override {
        this->_connected = false;
    }

    std::shared_ptr<mysqlx::Session> database() override {
        return this->_db;
    }
};