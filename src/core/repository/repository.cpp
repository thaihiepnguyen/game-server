#include "core/repository/repository.hpp"


void Repository::setDatabaseConnection(std::shared_ptr<IDatabaseConnection> dbConnection) {
    this->_dbConnection = dbConnection;
}

std::shared_ptr<IDatabaseConnection> Repository::db() const {
    if (!this->_dbConnection) {
        throw std::runtime_error("Database connection is not set.");
    }
    return this->_dbConnection;
}