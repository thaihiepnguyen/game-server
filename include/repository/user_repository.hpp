#include "core/repository/repository.hpp"
#include <iostream>

class UserRepository : public IRepository {
public:
    void createUser(const std::string& username, const std::string& password) {
        // Example implementation
        std::cout << "Creating user: " << username << " with password: " << password << "\n";
        // Here you would typically use the db() method to interact with the database
        // this->db()->execute("INSERT INTO users (username, password) VALUES (?, ?)", username, password);
    }
};