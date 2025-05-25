#pragma once
#include <memory>
#include "core/service/service.hpp"
#include "core/service/provider.hpp"
#include "service/auth_service.hpp"
#include "models/user.hpp"

class UserService : public IService {
private: 
    std::shared_ptr<AuthService> _authService;
public:
    void inject(std::shared_ptr<Provider> provider) override {
        _authService = provider->getService<AuthService>();
    }

    void createUser(User& user);

    bool isUserExists(const std::string& username);

    User getUserById(long long id);
};