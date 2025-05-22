#pragma once

#include "core/database/db_config.hpp"
#include "core/database/row_result.hpp"

class IDatabaseConnection {
protected:
    bool _connected = false;
    DbConfig _config;
public:
    IDatabaseConnection();
    virtual ~IDatabaseConnection() = default;

    virtual bool connect(const DbConfig& config) = 0;
    virtual void disconnect() = 0;
    virtual RowResult execute(const std::string& query) = 0;
    virtual bool isConnected() const = 0;
};