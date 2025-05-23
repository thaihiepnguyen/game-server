#pragma once
#include "core/command/command.hpp"
#include "core/service/provider.hpp"

class AuthCommand : public ICommand {
public:
    void inject(std::shared_ptr<Provider> provider) override {
        // Inject dependencies if needed
    }


    std::unordered_map<std::string, Protocol::Value> execute(const std::unordered_map<std::string, Protocol::Value>& request) override;
};