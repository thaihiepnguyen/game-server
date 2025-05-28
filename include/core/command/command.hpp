#pragma once

#include "core/protocol/protocol.hpp"
#include "core/service/provider.hpp"
#include "network/tcp_connection.hpp"
#include <unordered_map>
#include <memory>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual std::unordered_map<std::string, Protocol::Value> execute(
        const std::shared_ptr<TCPConnection>& connection,
        const std::unordered_map<std::string, Protocol::Value>& request
    ) = 0;
    virtual void inject(std::shared_ptr<Provider> provider) {};
};