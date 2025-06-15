#pragma once
#include "core/command/command.hpp"
#include "core/service/provider.hpp"
#include "service/user_service.hpp"
#include "service/auth_service.hpp"


class SignupCommand : public ICommand {
private:
    std::shared_ptr<AuthService> _authService;

public:
    void inject(std::shared_ptr<Provider> provider) override {
        _authService = provider->getService<AuthService>();
    }

    void execute(
        const std::shared_ptr<TCPConnection>& connection,
        Protocol::Packet& packet
    ) override;
};