#pragma once
#include "core/command/command.hpp"

class TestCommand : public ICommand {
public:
    std::unordered_map<std::string, Protocol::Value> execute(
        const std::shared_ptr<TCPConnection>& connection,
        const std::unordered_map<std::string, Protocol::Value>& request
    ) override {



        std::cout << "Executing TestCommand for user ID: " << connection->userId() << std::endl;
        return {};
    }
};