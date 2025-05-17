#include "database/db_config.hpp"

template <typename T>
class DBConnection {
protected:
    DbConfig _config;
    bool _connected;
    DBConnection();
public:
    virtual ~DBConnection() = default;

    virtual bool connect(const DbConfig& config) = 0;
    virtual void disconnect() = 0;
    bool isConnected() const;
    virtual std::shared_ptr<T> session() = 0;
};

template <typename T>
DBConnection<T>::DBConnection() : _config(), _connected(false) {}

template <typename T>
bool DBConnection<T>::isConnected() const {
    return this->_connected;
}