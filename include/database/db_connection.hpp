#pragma once

#include "database/db_config.hpp"
#include "database/row_result.hpp"

class DBConnection {
protected:
    bool _connected = false;
    DbConfig _config;
    DBConnection();
public:
    virtual ~DBConnection() = default;

    virtual bool connect(const DbConfig& config) = 0;
    virtual void disconnect() = 0;
    virtual RowResult execute(const std::string& query) = 0;
    virtual bool isConnected() const = 0;
};