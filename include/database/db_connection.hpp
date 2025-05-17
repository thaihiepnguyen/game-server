#include "database/db_config.hpp"

template <typename T>
class DBConnection {
protected:
    DbConfig _config;
    bool _connected;
public:
    DBConnection(const DbConfig& config);
    virtual ~DBConnection() = default;

    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    bool isConnected() const;
    virtual std::shared_ptr<T> database() = 0;
};

template <typename T>
DBConnection<T>::DBConnection(const DbConfig& config) {
    this->_config = config;
}

template <typename T>
bool DBConnection<T>::isConnected() const {
    return this->_connected;
}