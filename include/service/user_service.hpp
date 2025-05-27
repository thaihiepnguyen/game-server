#pragma once
#include <memory>
#include "core/service/service.hpp"
#include "core/service/provider.hpp"
#include "models/user.hpp"
#include <optional>

class UserService : public IService {
public:
    void createUser(User& user);

    bool isUserExists(const std::string& username);

    User getUserById(long long id);

    std::optional<User> getUserByUsername(const std::string& username);
};