#include "database/mysql/mysql_connection.hpp"
#include "core/utils/log.hpp"

MysqlConnection::~MysqlConnection() {
    // In case the connection is not closed, close it
    disconnect();
}

bool MysqlConnection::connect(const DbConfig& config) {
    if (this->_conn == NULL) {
        std::cerr << "MySQL connection is not initialized. \n";
        return false;
    }

    if (mysql_real_connect(
        this->_conn, 
        config.host().c_str(), 
        config.user().c_str(), 
        config.password().c_str(), 
        config.database().c_str(),
        config.port(), NULL, 0) == NULL
    ) {
        std::cerr << "mysql_real_connect() failed: " << mysql_error(this->_conn) << "\n";
        return false;
    }
    this->_connected = true;

    this->_config = config;
    Logger::logInfo("MySQL connection established: " + config.host() + ":" + std::to_string(config.port()));
    return true;
}

void MysqlConnection::disconnect() {
    if (!this->isConnected()) {
        return;
    }
    this->_connected = false;
    mysql_close(this->_conn);
}

RowResult MysqlConnection::execute(const std::string& query) {
    if (!this->isConnected()) {
        throw std::runtime_error("Not connected to the database.");
    }

    if (mysql_query(this->_conn, query.c_str())) {
        throw std::runtime_error("MySQL query failed: " + std::string(mysql_error(this->_conn)));
    }

    RowResult rowResult;
    MYSQL_RES* result = mysql_store_result(this->_conn);
    if (result == NULL) {
        return rowResult; // No result set
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        RowResult::Row rowData;
        for (unsigned int i = 0; i < mysql_num_fields(result); ++i) {
            if (row[i] == NULL) {
                rowData.push_back(std::monostate{});
                continue;
            }
            rowData.push_back(row[i]);
        }

        rowResult.addRow(rowData);
    }

    mysql_free_result(result);
    return rowResult;
}

bool MysqlConnection::isConnected() const {
    return this->_conn != NULL && this->_connected;
}