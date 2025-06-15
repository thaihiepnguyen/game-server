#pragma once

#include "core/protocol/protocol.hpp"
#include "core/service/provider.hpp"
#include "network/tcp_connection.hpp"
#include <unordered_map>
#include <memory>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(const std::shared_ptr<TCPConnection>& connection, Protocol::Packet& packet) = 0;
    virtual void inject(std::shared_ptr<Provider> provider) {};
};