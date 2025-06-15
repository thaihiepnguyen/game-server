#pragma once
#include "core/command/command.hpp"

class TestCommand : public ICommand {
public:
    void execute(
        const std::shared_ptr<TCPConnection>& connection,
        Protocol::Packet& packet
    ) override {
        std::cout << "Executing TestCommand for user ID: " << connection->userId() << std::endl;
    }
};