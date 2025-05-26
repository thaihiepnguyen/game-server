#pragma once
#include "core/command/command.hpp"
#include "core/service/provider.hpp"
#include "service/user_service.hpp"
#include "service/auth_service.hpp"

class AuthCommand : public ICommand {
private:
    std::shared_ptr<AuthService> _authService;
public:
    void inject(std::shared_ptr<Provider> provider) override {
        _authService = provider->getService<AuthService>();
    }

    std::unordered_map<std::string, Protocol::Value> execute(const std::unordered_map<std::string, Protocol::Value>& request) override;
};