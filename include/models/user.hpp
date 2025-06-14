#pragma once
#include <string>
#include "models/entity.hpp"
#include "core/database/db_result.hpp"
#include <variant>
#include <unordered_map>



class User : public Entity {
public:
    std::string username;
    std::string password;
    std::string salt;

    User() {}
    User(const std::string& username, const std::string& password, const std::string& salt)
        : username(username), password(password), salt(salt) {}

    static std::string tableName() {
        return "users";
    }

    static User fromRecord(const DBResult::Record& record) {
        User user;
        user.id = std::get<int64_t>(record.at("id"));
        user.username = std::get<std::string>(record.at("username"));
        user.salt = std::get<std::string>(record.at("salt"));
        user.password = std::get<std::string>(record.at("password_hash"));
        user.createdAt = std::get<std::string>(record.at("created_at"));
        user.updatedAt = std::get<std::string>(record.at("updated_at"));
        return user;
    }

    static std::unordered_map<std::string, std::string> toRecord(const User& user) {
        std::unordered_map<std::string, std::string> record;
        record["id"] = std::to_string(user.id);
        record["username"] = user.username;
        record["salt"] = user.salt;
        record["password_hash"] = user.password;
        return record;
    }
};