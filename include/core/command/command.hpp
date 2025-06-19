#pragma once

#include "core/service/provider.hpp"
#include "core/network/tcp_connection.hpp"
#include <unordered_map>
#include <memory>

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length) = 0;
    virtual void inject(std::shared_ptr<Provider> provider) {};
};