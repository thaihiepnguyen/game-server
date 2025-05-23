#pragma once
#include "core/command/command.hpp"
#include "core/service/provider.hpp"
#include "repository/user_repository.hpp"


class SignupCommand : public ICommand {
public:
    void inject(std::shared_ptr<Provider> provider) override {}

    std::unordered_map<std::string, Protocol::Value> execute(const std::unordered_map<std::string, Protocol::Value>& request) override;
};