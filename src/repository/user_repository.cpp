#include "repository/user_repository.hpp"


void UserRepository::createUser(const std::string& username, const std::string& password) {
    std::string query = "INSERT INTO users (username, password_hash) VALUES ('" + username + "', '" + password + "')";

    db()->execute(query);
    std::cout << "User created: " << username << std::endl;
}