#include "repository/user_repository.hpp"


void UserRepository::createUser(const std::string& username, const std::string& password, const std::string& salt) {
    std::string query = "INSERT INTO users (username, salt, password_hash) VALUES ('" 
    + username + "', '" + salt + "', '" + password + "')";

    db()->execute(query);
    std::cout << "User created: " << username << std::endl;
}