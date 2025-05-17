#include "database/mysql/mysql_connection.hpp"


MysqlConnection::~MysqlConnection() {
    // In case the destructor is called, we should ensure that the connection is closed.
    disconnect();
}

bool MysqlConnection::connect(const DbConfig& config) {
    try {
        this->_session = std::make_shared<mysqlx::Session>(config.host(), config.port(), config.user(), config.password());
        this->_config = config;

        if (!this->_session) {
            throw std::runtime_error("Failed to create MySQL session.");
        }

        this->_session->sql("USE " + config.database()).execute();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        this->_connected = false;
        return false;
    }
    this->_connected = true;
    return true;
}

void MysqlConnection::disconnect() {
    if (this->_connected && this->_session) {
        try {
            this->_session->close();
            this->_connected = false;
        } catch (const std::exception& e) {
            std::cerr << "Error closing session: " << e.what() << "\n";
        }
    }
}

std::shared_ptr<mysqlx::Session> MysqlConnection::session() {
    if (!this->_connected || !this->_session) {
        throw std::runtime_error("Not connected to the database.");
    }
    return this->_session;
}

std::shared_ptr<MysqlConnection> MysqlConnection::instance() {
    if (!_instance) {
        _instance = std::shared_ptr<MysqlConnection>(new MysqlConnection());
    }
    return _instance;
}