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

DBResult MysqlConnection::execute(const std::string& query) {
    if (!this->isConnected()) {
        throw std::runtime_error("Not connected to the database.");
    }

    if (mysql_query(this->_conn, query.c_str())) {
        throw std::runtime_error("MySQL query failed: " + std::string(mysql_error(this->_conn)));
    }

    DBResult rowResult;
    MYSQL_RES* result = mysql_store_result(this->_conn);
    if (result == NULL) {
        return rowResult; // No result set
    }

    unsigned int num_fields = mysql_num_fields(result);
    MYSQL_FIELD* fields = mysql_fetch_fields(result);
    std::vector<std::string> column_names;
    for (unsigned int i = 0; i < num_fields; ++i) {
        column_names.push_back(fields[i].name);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        DBResult::Record rowData;
        for (unsigned int i = 0; i < num_fields; ++i) {
            if (row[i] == NULL) {
                rowData[column_names[i]] = std::monostate{};
            } else {
                switch (fields[i].type) {
                    // Integer types
                    case MYSQL_TYPE_TINY:
                    case MYSQL_TYPE_SHORT:
                    case MYSQL_TYPE_LONG:
                    case MYSQL_TYPE_LONGLONG:
                    case MYSQL_TYPE_INT24:
                    case MYSQL_TYPE_YEAR:
                        rowData[column_names[i]] = std::stoll(row[i]);
                        break;

                    // Floating-point types
                    case MYSQL_TYPE_FLOAT:
                    case MYSQL_TYPE_DOUBLE:
                    case MYSQL_TYPE_DECIMAL:
                    case MYSQL_TYPE_NEWDECIMAL:
                        rowData[column_names[i]] = std::stod(row[i]);
                        break;

                    // String and text types
                    case MYSQL_TYPE_VARCHAR:
                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_VAR_STRING:
                    case MYSQL_TYPE_JSON:
                    case MYSQL_TYPE_ENUM:
                    case MYSQL_TYPE_SET:
                        rowData[column_names[i]] = std::string(row[i]);
                        break;

                    // Binary types (treat as string for simplicity)
                    case MYSQL_TYPE_TINY_BLOB:
                    case MYSQL_TYPE_MEDIUM_BLOB:
                    case MYSQL_TYPE_LONG_BLOB:
                    case MYSQL_TYPE_BLOB:
                        rowData[column_names[i]] = std::string(row[i]);
                        break;

                    // Temporal types (store as string)
                    case MYSQL_TYPE_TIMESTAMP:
                    case MYSQL_TYPE_DATE:
                    case MYSQL_TYPE_TIME:
                    case MYSQL_TYPE_DATETIME:
                    case MYSQL_TYPE_TIMESTAMP2:
                    case MYSQL_TYPE_DATETIME2:
                    case MYSQL_TYPE_TIME2:
                        rowData[column_names[i]] = std::string(row[i]);
                        break;

                    // Boolean
                    case MYSQL_TYPE_BOOL:
                        rowData[column_names[i]] = (std::string(row[i]) == "1");
                        break;

                    // Bit type (store as string or convert to integer if needed)
                    case MYSQL_TYPE_BIT:
                        rowData[column_names[i]] = std::string(row[i]);
                        break;

                    // Geometry (store as string for simplicity)
                    case MYSQL_TYPE_GEOMETRY:
                        rowData[column_names[i]] = std::string(row[i]);
                        break;

                    // Internal or replication-only types
                    case MYSQL_TYPE_NEWDATE:
                    case MYSQL_TYPE_TYPED_ARRAY:
                        rowData[column_names[i]] = std::string(row[i]);
                        break;

                    // Invalid or unsupported
                    case MYSQL_TYPE_INVALID:
                    default:
                        throw std::runtime_error("Unsupported field type: " + std::to_string(fields[i].type));
                }
            }
        }
        rowResult.addRow(rowData);
    }

    mysql_free_result(result);
    return rowResult;
}

bool MysqlConnection::isConnected() const {
    return this->_conn != NULL && this->_connected;
}