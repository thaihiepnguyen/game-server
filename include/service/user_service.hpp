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

    std::optional<User> getUserByUsername(const std::string& username) {
        auto db = this->repository()->db();
        if (!db->isConnected()) {
            throw std::runtime_error("Database connection is not established");
        }

        std::string query = "SELECT * FROM " + User::tableName() + " WHERE username = '" + username + "'";
        DBResult result = db->execute(query);

        if (result.size() == 0) {
            return std::nullopt; // User not found
        }

        return User::fromRecord(result[0]);
    }
};