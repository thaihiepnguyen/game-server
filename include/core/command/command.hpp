#pragma once

#include "core/protocol/protocol.hpp"

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual std::map<std::string, Protocol::Value> execute(const std::map<std::string, Protocol::Value>& request) = 0;
};