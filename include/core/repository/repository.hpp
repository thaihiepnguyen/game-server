#pragma once
#include <string>
#include <vector>
#include "core/database/db_connection.hpp"

class IRepository {
private:
    std::shared_ptr<IDatabaseConnection> _dbConnection;

public:
    virtual ~IRepository() = default;
    void setDatabaseConnection(std::shared_ptr<IDatabaseConnection> dbConnection);

    std::shared_ptr<IDatabaseConnection> db() const;
};