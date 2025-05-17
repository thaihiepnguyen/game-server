#pragma once

#include "database/db_connection.hpp"
#include <mysqlx/xdevapi.h>
#include <memory>
#include <iostream>
#include <stdexcept>

class MysqlConnection : public DBConnection<mysqlx::Session> {
private:
    inline static std::shared_ptr<MysqlConnection> _instance = NULL;
    std::shared_ptr<mysqlx::Session> _session;
    MysqlConnection() : _session(NULL) {}
public:
    ~MysqlConnection() override;

    bool connect(const DbConfig& config) override;
    void disconnect() override;
    std::shared_ptr<mysqlx::Session> session() override;
    static std::shared_ptr<MysqlConnection> instance();
};