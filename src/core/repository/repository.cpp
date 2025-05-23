#include "core/repository/repository.hpp"


void IRepository::setDatabaseConnection(std::shared_ptr<IDatabaseConnection> dbConnection) {
    this->_dbConnection = dbConnection;
}

std::shared_ptr<IDatabaseConnection> IRepository::db() const {
    if (!this->_dbConnection) {
        throw std::runtime_error("Database connection is not set.");
    }
    return this->_dbConnection;
}