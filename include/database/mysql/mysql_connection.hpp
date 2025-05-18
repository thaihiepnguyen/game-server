#pragma once

#include "database/db_connection.hpp"
#include <memory>
#include <iostream>
#include <stdexcept>
#include <mysql/mysql.h>

class MysqlConnection : public DBConnection {
private:
    inline static std::shared_ptr<DBConnection> _instance = NULL;
    MYSQL* _conn;
    MysqlConnection() : _conn(mysql_init(NULL)) {}
public:
    ~MysqlConnection() override;

    bool connect(const DbConfig& config) override;
    void disconnect() override;
    RowResult execute(const std::string& query) override;
    bool isConnected() const override;

    static std::shared_ptr<DBConnection> instance();
};