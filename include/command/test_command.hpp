#pragma once
#include "core/command/command.hpp"

class TestCommand : public ICommand {
public:
    std::unordered_map<std::string, Protocol::Value> execute(const std::unordered_map<std::string, Protocol::Value>& request) override {
        std::unordered_map<std::string, Protocol::Value> response;
        response["status"] = Protocol::Value("success");
        response["message"] = Protocol::Value("Test command executed successfully");
        return response;
    }
};