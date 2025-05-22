#pragma once

#include "core/database/db_connection.hpp"
#include <memory>
#include <iostream>
#include <stdexcept>
#include <mysql/mysql.h>


class MysqlConnection : public IDatabaseConnection {
private:
    MYSQL* _conn;
public:
    MysqlConnection() : _conn(mysql_init(NULL)) {}
    ~MysqlConnection() override;
    
    bool connect(const DbConfig& config) override;
    void disconnect() override;
    RowResult execute(const std::string& query) override;
    bool isConnected() const override;
};